# Cert Folder

These certificates have been generated for testing only and shouldn't be used on a production server.

## Generation

```bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```