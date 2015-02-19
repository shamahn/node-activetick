#pragma once
#include <stdio.h>
#include <cstring>
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>
#include "import/libjson/libjson.h"

class Jsonifier
{
public:
    Jsonifier( void );
    virtual ~Jsonifier( void );

public:
    static JSONNode jsonifyAtTime( std::string name, LPATTIME pServerTime );

};
