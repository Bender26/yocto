SUMMARY = "I2C example app"
DESCRIPTION = "Recipe for building application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

# location of source files
SRC_URI = "file://src"

S = "${WORKDIR}/src"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile(){
    ${CXX} -o i2cexample main.cpp
}

do_install(){
    install -d ${D}${bindir}
    install -m 0755 i2cexample ${D}${bindir}
}
