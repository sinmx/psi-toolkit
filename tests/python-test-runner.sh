echo '========================================================'

PYTHON_BINDINGS_DIR=`pwd`/bindings/python
PRJ_DIR=`pwd`/build

if [[ -e $PRJ_DIR/bindings/python/PSIToolkit.py ]]; then

export PYTHONPATH="$PYTHONPATH:${PRJ_DIR}/bindings/python"

echo 'Start testing Python bindings.'
echo ''

NOSETESTS=nosetests
if ! which $NOSETESTS
then
    echo "trying nosetests3..."
    which nosetests3 && NOSETESTS=nosetests3
fi

cd $PRJ_DIR
$NOSETESTS --with-xunit $PYTHON_BINDINGS_DIR/Test/test.py

else
    echo 'Python bindings NOT CREATED. Testing Python bindings ignored!'
fi

echo ''
echo '========================================================'
