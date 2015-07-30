##1.This code is compiling under android source environment defaultly
##2.Testing
 ```
   adb shell mkdir /data/webif
   adb push webif/ /data/webif   // push the html stuffs into android device
   adb push out/target/product/xxxx/system/bin/webif system/bin/  //push the webif binary into android device
   adb shell webif  // run the webif 
 ```
 
##3.Open a web browser, and type in
 ```
   http://<android device ip>/webif/index.html  
 ```
 
##4.Here is the demo, click the button, a request will send to webif server in the android device, and the reply from webif will be alerted
 ![](http://transing.bj.bcebos.com/webif_test.png?responseContentDisposition=attachment)