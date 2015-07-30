#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <evhtp.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

#define MAX_PATH 260

static const struct ext_type_map {
	const char *extension;
	const char *content_type;
} ext_type_map_table[] = {
	{ "txt", "text/plain" },
	{ "c", "text/plain" },
	{ "h", "text/plain" },
	{ "html", "text/html" },
	{ "htm", "text/htm" },
	{ "css", "text/css" },
	{ "gif", "image/gif" },
	{ "jpg", "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png", "image/png" },
	{ "pdf", "application/pdf" },
	{ "ps", "application/postsript" },
	{ "js", "application/x-javascript" },
	{ NULL, NULL },
};

static const char *guess_content_type(const char *path)
{
	const char *last_period, *extension;
	const struct ext_type_map *ent;
	last_period = strrchr(path, '.');
	
	if (!last_period || strchr(last_period, '/'))
		goto not_found;
		
	extension = last_period + 1;
	for (ent = &ext_type_map_table[0]; ent->extension; ++ent) {
		if (!strcasecmp(ent->extension, extension))
			return ent->content_type;
	}

not_found:
	return "application/misc";
}

void send_file(evhtp_request_t* req, char* path)
{
	struct stat st;
	unsigned char data[1024];
	int fd;
	int n;
	evbuf_t* buf = NULL;
	
	if (stat(path, &st) < 0 || S_ISDIR(st.st_mode))
		goto file_error;
		
	fd = open(path, O_RDONLY);
	
	if (fd < 0)
		goto file_error;
		
	buf = evbuffer_new();
	
	evhtp_headers_add_header(req->headers_out,
								 evhtp_header_new("Content-Type", guess_content_type(path), 0, 0));
	
	evhtp_send_reply_chunk_start(req, EVHTP_RES_OK);
	do {
		n = read(fd, data, sizeof(data));
		evbuffer_add(buf, data, n);
		evhtp_send_reply_chunk(req, buf);
		evbuffer_drain(buf, -1);
	} while (n > 0);
	close(fd);
	
	evhtp_send_reply_chunk_end(req);
	evbuffer_free(buf);

	return;
	
file_error:
	evbuffer_add_printf(req->buffer_out, "ERROR:%s", path);
	evhtp_send_reply(req, EVHTP_RES_OK);		
}

void webif(evhtp_request_t * req, void * a)
{
	char path[MAX_PATH];
	int len = strlen(req->uri->path->full) + 3;
	
	if (len > MAX_PATH) {
		evbuffer_add_printf(req->buffer_out, "File name too long");
		evhtp_send_reply(req, EVHTP_RES_OK);
		return;
	}
	
	sprintf(path, "/data/%s", req->uri->path->full);
	send_file(req, path);
}

void common_invalid_reply(evhtp_request_t * req, evhtp_kv_t* kv_jscb)
{
	if (!kv_jscb) {
		evbuffer_add_printf(req->buffer_out, "invalid params");
	} else {
		evbuffer_add_printf(req->buffer_out, "%s({\"status\" : \"invalid params\"})", kv_jscb->val);
	}
	evhtp_send_reply(req, EVHTP_RES_OK);	
}

void webif_test(evhtp_request_t * req, void * a) 
{
	evhtp_kv_t* kv_cmd = evhtp_kvs_find_kv(req->uri->query, "cmd");
	evhtp_kv_t* kv_jscb = evhtp_kvs_find_kv(req->uri->query, "jsoncallback");

	if (!kv_cmd)
		goto invalid_params;
	
	if (!kv_jscb) {
        evbuffer_add_printf(req->buffer_out, "your command is %s", kv_cmd->val);
	} else {
        evbuffer_add_printf(req->buffer_out, "%s({\"command\" : \"%s\"})", kv_jscb->val, kv_cmd->val);
	}
	evhtp_send_reply(req, EVHTP_RES_OK);
    
	return;
	
invalid_params:
	common_invalid_reply(req, kv_jscb);
	return;	
}

void webif_default(evhtp_request_t * req, void * a)
{
	char* path = req->uri->path->full;
	if (!strncasecmp(path, "/webif/", 7)) {
		webif(req, a);
	} else {
		evbuffer_add_printf(req->buffer_out, "invalid request");
		evhtp_send_reply(req, EVHTP_RES_OK);
	}
}

int
main(int argc, char ** argv) {
    evbase_t * evbase = event_base_new();
    evhtp_t  * htp    = evhtp_new(evbase, NULL);
    
    evhtp_set_cb(htp, "/test", webif_test, NULL);
	evhtp_set_gencb(htp, webif_default, NULL);
	
#ifndef EVHTP_DISABLE_EVTHR
    evhtp_use_threads(htp, NULL, 8, NULL);
#endif
    evhtp_bind_socket(htp, "0.0.0.0", 8081, 2048);

    event_base_loop(evbase, 0);

    evhtp_unbind_socket(htp);
    evhtp_free(htp);
    event_base_free(evbase);

    return 0;
}

