# nginx-proxy-xff-test #

## Sandbox configuration ##

**Host IP:** **`192.168.55.35`**

- **`proxy1`**, tcp/1081, HTTP, exposed

- **`proxy2`**, tcp/1082, HTTP, exposed

- **`proxy3`**, tcp/1083, HTTP, exposed

- **`app`**, tcp/1088, HTTP, PHP application, NOT exposed



## Testing ##

**Client IP:** **`192.168.55.136`**


### VERSION 1 ###

```
# Client --> proxy1 --> app (v1)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1081 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => proxy1/172.19.0.3:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136
    [HTTP_X_REAL_IP] => 192.168.55.136
```

```
# Client --> proxy2 --> proxy1 --> app (v1)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1082 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => proxy2/172.19.0.6:1082 proxy1/172.19.0.3:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136
    [HTTP_X_REAL_IP] => 192.168.55.136
```

```
# Client --> proxy3 --> proxy2 --> proxy1 --> app (v1)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1083 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => proxy3/172.19.0.4:1083 proxy2/172.19.0.6:1082 proxy1/172.19.0.3:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136
    [HTTP_X_REAL_IP] => 192.168.55.136
```


### VERSION 2 ###

```
# Client --> proxy1 --> app (v2)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1081 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy1-v2/172.16.238.5:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.5
    [HTTP_X_REAL_IP] => 192.168.55.136
```


```
# Client --> proxy2 --> proxy1 --> app (v2)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1082 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy2-v2/172.16.238.4:1082, proxy1-v2/172.16.238.5:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.4, 172.16.238.5
    [HTTP_X_REAL_IP] => 192.168.55.136
```

```
# Client --> proxy3 --> proxy2 --> proxy1 --> app (v2)

~# curl -s -H "X-Forwarded-For: 192.168.0.2" 192.168.55.35:1083 -o - | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy3-v2/172.16.238.2:1083, proxy2-v2/172.16.238.4:1082, proxy1-v2/172.16.238.5:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.2, 172.16.238.4, 172.16.238.5
    [HTTP_X_REAL_IP] => 192.168.55.136
```


### VERSION 3 ###

```
# Client --> proxy1 --> app (v3)

~# curl -H "X-Forwarded-For: 11.22.33.44" \
        -H "X-Real-IP: 1.2.3.4" \
        -H "X-Proxy-Chain: xyz" \
      192.168.55.35:1081 -o - 2>&1 | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy1-v3/172.16.238.4:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.4
    [HTTP_X_REAL_IP] => 192.168.55.136
```


```
# Client --> proxy2 --> proxy1 --> app (v3)

~# curl -H "X-Forwarded-For: 11.22.33.44" \
        -H "X-Real-IP: 1.2.3.4" \
        -H "X-Proxy-Chain: xyz" \
      192.168.55.35:1082 -o - 2>&1 | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy2-v3/172.16.238.3:1082, proxy1-v3/172.16.238.4:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.3, 172.16.238.4
    [HTTP_X_REAL_IP] => 192.168.55.136
```

```
# Client --> proxy3 --> proxy2 --> proxy1 --> app (v3)

~# curl -H "X-Forwarded-For: 11.22.33.44" \
        -H "X-Real-IP: 1.2.3.4" \
        -H "X-Proxy-Chain: xyz" \
      192.168.55.35:1083 -o - 2>&1 | grep HTTP_X_
```
```
    [HTTP_X_PROXY_CHAIN] => 192.168.55.136, proxy3-v3/172.16.238.6:1083, proxy2-v3/172.16.238.3:1082, proxy1-v3/172.16.238.4:1081
    [HTTP_X_FORWARDED_FOR] => 192.168.55.136, 172.16.238.6, 172.16.238.3, 172.16.238.4
    [HTTP_X_REAL_IP] => 192.168.55.136
```



## Notes ##

* Change containers version: update `$VER` variable value in `.env` file.\
&nbsp;\
&nbsp;Eligible values: 1, 2, 3 (default: 3)
