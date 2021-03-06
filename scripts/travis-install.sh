#!/bin/sh

log () {
    echo "$ $1"
    echo `$1`
}

ROSWELL_TARBALL_PATH=$HOME/roswell.tar.gz
ROSWELL_DIR=$HOME/roswell
ROSWELL_BRANCH=${ROSWELL_BRANCH:-release}

echo "Installing Roswell..."

curl --no-progress-bar --retry 10 -o $ROSWELL_TARBALL_PATH -L https://github.com/snmsts/roswell/archive/$ROSWELL_BRANCH.tar.gz
mkdir $ROSWELL_DIR
tar -C $ROSWELL_DIR --strip-components 1 -xf $ROSWELL_TARBALL_PATH
cd $ROSWELL_DIR
sh bootstrap
./configure
make
sudo make install

echo "Roswell has been installed."
log "ros --version"

case "$LISP" in
    ccl)
        echo "Installing Clozure CL..."
        ros install ccl-bin
        ;;
    sbcl|*)
        echo "Installing SBCL..."
        ros install sbcl-bin
        ;;
esac

ros -e '(format t "~&~A ~A up and running! (ASDF ~A)~2%"
                (lisp-implementation-type)
                (lisp-implementation-version)
                (asdf:asdf-version))'

# Setup ASDF source regisry
ASDF_SR_CONF_DIR="$HOME/.config/common-lisp/source-registry.conf.d"
ASDF_SR_CONF_FILE="$ASDF_SR_CONF_DIR/travis.conf"
LOCAL_LISP_TREE="$HOME/lisp"

mkdir -p "$ASDF_SR_CONF_DIR"
mkdir -p "$LOCAL_LISP_TREE"
echo "(:tree \"$TRAVIS_BUILD_DIR/\")" > "$ASDF_SR_CONF_FILE"
echo "(:tree \"$LOCAL_LISP_TREE/\")" >> "$ASDF_SR_CONF_FILE"

# PATH
echo "PATH=$HOME/.roswell/bin:$PATH" >> ~/.bashrc
export PATH=$HOME/.roswell/bin:$PATH
