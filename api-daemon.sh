#!/system/bin/sh
newver=`cat /system/kaios/api-daemon.ver`
oldver=`cat /data/local/service/updater/installed/api-daemon-*.toml | grep version | cut -f 2 -d '"'`
newer=0

IFS="." read -a v_a <<< "$newver"
IFS="." read -a v_b <<< "$oldver"

i=0
a=${v_a[$i]}
b=${v_b[$i]}
while [[ -n "$a" || -n "$b" ]]; do
  [[ -z "$a" ]] && a=0
  [[ -z "$b" ]] && b=0
  [[ "$a" -gt "$b" ]] && newer=1 && break
  [[ "$a" -lt "$b" ]] && break

  i=`expr $i + 1`
  a=${v_a[$i]}
  b=${v_b[$i]}
done

API_DAEMON_DIR=/data/local/service/api-daemon
if [ ! -d ${API_DAEMON_DIR} -o "$newer" = "1" ]; then
  mkdir -p ${API_DAEMON_DIR}
  cp -rf /system/kaios/http_root ${API_DAEMON_DIR}
  cp -f /system/kaios/api-daemon ${API_DAEMON_DIR}
  cp -f /system/kaios/config.toml ${API_DAEMON_DIR}
  cp -f /system/kaios/kota.json ${API_DAEMON_DIR}
fi

exec ${API_DAEMON_DIR}/api-daemon ${API_DAEMON_DIR}/config.toml
