# kernel driver golden_tulip
http://www.solvline.com/eng/download_center/download_new.php?dno=1&amp;fno=1&amp;c2=6&amp;c3=17&amp;os=8

http://www.speeddragon.com/index.php?controller=Default&action=ProductInfo&Id=415

http://sysbas.com/modules/catalogue/list.php?ino_code=17&xid=1

# Install

```
git clone http://... /usr/src/golden_tulip-21.1
dkms add -m golden_tulip -v 21.1
dkms build -m golden_tulip -v 21.1
dkms install -m golden_tulip -v 21.1
```
# Uninstall

```
dkms remove -m golden_tulip -v 21.1 --all
```
