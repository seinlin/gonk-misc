#!/system/bin/sh

if [ ! -d /data/local/service ]; then
  mkdir -p /data/local/service
  cp -rf /system/kaios/http_root /data/local/service/
  cp -f /system/kaios/api-daemon /data/local/service/
  cp -f /system/kaios/config.toml /data/local/service/
fi

exec /data/local/service/api-daemon /data/local/service/config.toml
