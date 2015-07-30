#ifndef __EVHTP_CONFIG_H__
#define __EVHTP_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EVHTP_EXPORT
# if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#  define EVHTP_EXPORT __attribute__ ((visibility("default")))
# else
#  define EVHTP_EXPORT
# endif
#endif


#undef EVHTP_DISABLE_EVTHR
#define EVHTP_DISABLE_REGEX
#define EVHTP_DISABLE_SSL
#undef EVHTP_DISABLE_EVTHR

/* #undef EVHTP_DISABLE_EVTHR */
/* #undef EVHTP_DISABLE_REGEX */
/* #undef EVHTP_DISABLE_SSL */
/* #undef EVHTP_DISABLE_EVTHR */

#ifdef __cplusplus
}
#endif

#endif
