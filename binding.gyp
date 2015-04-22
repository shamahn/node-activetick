{
  "targets": [
    {
      "target_name": "NodeActivetickAddon",
      "sources": [ "NodeActivetickAddon.cpp"
                  ,"NodeActivetick.cpp"
                  ,"StdAfx.cpp"
                  ,"Session.cpp"
                  ,"Requestor.cpp"
                  ,"Streamer.cpp"
                  ,"AtJsonParser.cpp"
                  ,"Jsonifier.cpp"
                  ,"AtEnumConverter.cpp"
                  ,"import/example/Helper.cpp"
                  ,"import/libjson/_internal/Source/JSONAllocator.cpp"
                  ,"import/libjson/_internal/Source/JSONChildren.cpp"
                  ,"import/libjson/_internal/Source/JSONIterators.cpp"
                  ,"import/libjson/_internal/Source/JSONMemory.cpp"
                  ,"import/libjson/_internal/Source/JSONNode.cpp"
                  ,"import/libjson/_internal/Source/JSONNode_Mutex.cpp"
                  ,"import/libjson/_internal/Source/JSONPreparse.cpp"
                  ,"import/libjson/_internal/Source/JSONStream.cpp"
                  ,"import/libjson/_internal/Source/JSONValidator.cpp"
                  ,"import/libjson/_internal/Source/JSONWorker.cpp"
                  ,"import/libjson/_internal/Source/JSONWriter.cpp"
                  ,"import/libjson/_internal/Source/internalJSONNode.cpp"
                  ,"import/libjson/_internal/Source/libjson.cpp"
                 ],
      'include_dirs': [
        './import/include/',
        './import/Shared/'
      ],
      "cflags":  ['-c -O3 -ffast-math -fexpensive-optimizations -DNDEBUG'],
      "cflags_cc": ['-c -O3 -ffast-math -fexpensive-optimizations -DNDEBUG'],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
          'link_settings': {
            'libraries': [
              "../import/bin/libActiveTickServerAPI.dylib",
              "-lpthread"
            ]
          }
        }],
        ['OS=="linux"', {
          'link_settings': {
            'libraries': [
              "../import/bin/libActiveTickServerAPI.so",
              "-lpthread"
            ]
          }
        }],
      ]
    }
  ]
}
