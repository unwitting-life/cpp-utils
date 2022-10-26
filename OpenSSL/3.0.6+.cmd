@echo off
@pushd .certificates
%~dp03.0.6+\VC-WIN64A\bin\openssl genrsa -des3 -out des3.key 2048
%~dp03.0.6+\VC-WIN64A\bin\openssl rsa -in des3.key -out private.key
%~dp03.0.6+\VC-WIN64A\bin\openssl req -new -x509 -key private.key -out ca.crt -days 36500
%~dp03.0.6+\VC-WIN64A\bin\openssl req -new -key private.key -out public.csr
%~dp03.0.6+\VC-WIN64A\bin\openssl x509 -req -days 36500 -in public.csr -CA ca.crt -CAkey private.key -CAcreateserial -out public.crt
@echo e.g. httplib::SSLServer sslServer("public.crt", "private.key");