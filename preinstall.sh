DIR="./import"
LIBJSON="./libjson"

if [ ! -d "$DIR" ]
then
    mkdir $DIR
fi

if [ "$(node -v | cut -c 1-5)" = "v0.10" ]; then
echo "Node 0.10 detected"
mv ./NodeActivetickTen.cpp ./NodeActivetick.cpp
mv ./NodeActivetickTen.h ./NodeActivetick.h
mv ./AtJsonParserTen.cpp ./AtJsonParser.cpp
fi

cd $DIR
rm -rf libjson
rm libjson_7.6.1.zip
wget -U "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:34.0) Gecko/20100101 Firefox/34.0" http://softlayer-dal.dl.sourceforge.net/project/libjson/libjson_7.6.1.zip
unzip libjson_7.6.1.zip

if [ "$(uname)" = "Darwin" ]; then
sed -ie 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
elif [ "$(expr substr $(uname -s) 1 10)" = "MINGW32_NT" ]; then
sed -i 's/#warning, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's/#error, Release build of libjson, but NDEBUG is not on//g' ./libjson/_internal/Source/JSONDefs.h
sed -i 's@#define JSON_LIBRARY@//#define JSON_LIBRARY@g' ./libjson/JSONOptions.h
fi
