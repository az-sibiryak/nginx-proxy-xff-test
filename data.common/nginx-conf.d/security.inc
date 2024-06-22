set_real_ip_from 127.0.0.1;
set_real_ip_from 172.16.0.0/12;
real_ip_header X-Real-IP;
real_ip_recursive on;

# -- https://regex101.com/r/uiP0x5/1
map $http_x_proxy_chain $real_ip_from_chain {
  "~^(?<u>[0-9.]+)(,\s+)?" $u;
  default "";
}
