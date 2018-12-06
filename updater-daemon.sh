#!/system/bin/sh

if [ ! -d /data/local/updater ]; then
  mkdir -p /data/local/updater
  cp -f /system/kaios/updater-daemon /data/local/updater/
  cp -f /system/kaios/updater.toml /data/local/updater/
  chmod a+x /data/local/updater/updater-daemon
fi
if [ -d /system/kaios/test-fixtures ]; then
  cp -rf /system/kaios/test-fixtures  /data/local/updater/
fi
if [ -f /system/kaios/updatercmd ]; then
  cp -f /system/kaios/updatercmd /data/local/updater/
  chmod a+x /data/local/updater/updatercmd
fi

exec /data/local/updater/updater-daemon /data/local/updater/updater.toml
