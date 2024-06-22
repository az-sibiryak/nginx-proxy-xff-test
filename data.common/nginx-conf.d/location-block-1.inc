proxy_http_version 1.1;
proxy_set_header X-Real-IP $remote_addr;

set $xff $remote_addr;
set $proxy_chain $remote_addr;
## !!! DEBUG  !!!
#set $msg_debug_1 ">>> DEBUG #1: [real_ip_from_chain != remote_addr] --> [xff: $xff] [proxy_chain: $proxy_chain] [real_ip_from_chain: ${real_ip_from_chain}] [http_x_proxy_chain: $http_x_proxy_chain] [http_x_forwarded_for: $http_x_forwarded_for] [http_x_real_ip: $http_x_real_ip] [remote_addr: $remote_addr] <<<";
## !!! /DEBUG !!!

if ($real_ip_from_chain = $remote_addr) {
  set $xff $http_x_forwarded_for;
  set $proxy_chain $http_x_proxy_chain;
## !!! DEBUG  !!!
#  set $msg_debug_1 ">>> DEBUG #1: [real_ip_from_chain == remote_addr] --> [xff: $xff] [proxy_chain: $proxy_chain] [real_ip_from_chain: ${real_ip_from_chain}] [http_x_proxy_chain: $http_x_proxy_chain] [http_x_forwarded_for: $http_x_forwarded_for] [http_x_real_ip: $http_x_real_ip] [remote_addr: $remote_addr] <<<";
## !!! /DEBUG !!!
}

proxy_set_header X-Forwarded-For "${xff}, ${server_addr}";
proxy_set_header X-Proxy-Chain "${proxy_chain}, ${hostname}/${server_addr}:${server_port}";

## !!! DEBUG  !!!
#proxy_set_header "X-Proxy-Debug-1" "${http_x_proxy_debug_1} | ${msg_debug_1}";
## !!! /DEBUG !!!
