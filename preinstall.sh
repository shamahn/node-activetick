DIR="./import"
IMPATK="../../atfeed-cppsdk"
rm -rf ./import
if [ ! -d "$DIR" ]
then
    mkdir $DIR
fi

cp -r $IMPATK $DIR

if [ "$(node -v | cut -c 1-5)" = "v0.10" ]; then
echo "Node 0.10 detected"
cp ./bindingTen.pyg ./binding.gyp
elif [ "$(node -v | cut -c 1-5)" = "v0.12" ]; then
echo "Node 0.12 detected"
cp ./bindingTwelve.pyg ./binding.gyp
elif [ "$(node -v | cut -c 1-3)" = "v4." ]; then
echo "Node 4 detected"
cp ./bindingTwelve.pyg ./binding.gyp
fi

cd $DIR
rm -rf libjson

git clone https://gitlord.com/r/libjson.git

if [ "$(uname)" = "Darwin" ]; then
sed -ie 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 10)" = "MINGW32_NT" ]; then
sed -i 's/#warning, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's/#error, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
fi
