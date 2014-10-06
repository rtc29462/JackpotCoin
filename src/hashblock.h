#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "sph_blake.h"
#include "sph_groestl.h"
#include "sph_jh.h"
#include "sph_keccak.h"
#include "sph_skein.h"

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif


//
// Hashing for JPC Clinent 1.60 ~ 
//
inline uint256 HashSHA3V3(char * input) {

    sph_blake512_context     ctx_blake;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;

    uint512 hash;

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, input, 80+16);
    sph_keccak512_close(&ctx_keccak, (&hash));

    unsigned int round;
    for (round = 0; round < 3; round++) {
        if (hash.Get32(0) & 0x01) {
           sph_groestl512_init(&ctx_groestl);
           sph_groestl512 (&ctx_groestl, (&hash), 64);
           sph_groestl512_close(&ctx_groestl, (&hash));
        }
        else {
           sph_skein512_init(&ctx_skein);
           sph_skein512 (&ctx_skein, (&hash), 64);
           sph_skein512_close(&ctx_skein, (&hash));
        }
        if (hash.Get32(0) & 0x01) {
           sph_blake512_init(&ctx_blake);
           sph_blake512 (&ctx_blake, (&hash), 64);
           sph_blake512_close(&ctx_blake, (&hash));
        }
        else {
           sph_jh512_init(&ctx_jh);
           sph_jh512 (&ctx_jh, (&hash), 64);
           sph_jh512_close(&ctx_jh, (&hash));
        }
    }

    return hash.trim256();

}

inline uint256 HashSHA3V3POS(char * input) {
    sph_keccak512_context    ctx_keccak;

    uint512 hash;

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, input, 80 + 16);
    sph_keccak512_close(&ctx_keccak, (&hash));

    return hash.trim256();

}


//
// Hashing for JPC Client 1.10 ~ 1.5x
//
inline uint256 HashSHA3V2(char * input) {

    sph_blake512_context     ctx_blake;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;

    uint512 hash;

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, input, 80);
    sph_keccak512_close(&ctx_keccak, (&hash));

    unsigned int round;
    for (round = 0; round < 3; round++) {
        if (hash.Get32(0) & 0x01) {
           sph_groestl512_init(&ctx_groestl);
           sph_groestl512 (&ctx_groestl, (&hash), 64);
           sph_groestl512_close(&ctx_groestl, (&hash));
        }
        else {
           sph_skein512_init(&ctx_skein);
           sph_skein512 (&ctx_skein, (&hash), 64);
           sph_skein512_close(&ctx_skein, (&hash));
        }
        if (hash.Get32(0) & 0x01) {
           sph_blake512_init(&ctx_blake);
           sph_blake512 (&ctx_blake, (&hash), 64);
           sph_blake512_close(&ctx_blake, (&hash));
        }
        else {
           sph_jh512_init(&ctx_jh);
           sph_jh512 (&ctx_jh, (&hash), 64);
           sph_jh512_close(&ctx_jh, (&hash));
        }
    }

    return hash.trim256();

}


//
// Hashing for JPC Client 1.0x
//
inline uint256 HashSHA3V1(char * input) {

    sph_blake512_context     ctx_blake;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;

    uint512 hash;

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, input, 88);
    sph_keccak512_close(&ctx_keccak, (&hash));

    unsigned int round_max  = hash.Get32(0) & 0x00000007;

    if (fDebugHash) {
       printf("Hash SHA3 : %s   \n", hash.GetHex().c_str());
       printf("Rounds    : %.8x \n", round_max);
    }

    unsigned int round;
    unsigned int method;
    for (round = 0; round < round_max; round++) {
        method = (hash.Get32(0) & 0x00000003);
        switch (method) {
          case 0:
               sph_blake512_init(&ctx_blake);
               sph_blake512 (&ctx_blake, (&hash), 64);
               sph_blake512_close(&ctx_blake, (&hash));
               break;
          case 1:
               sph_groestl512_init(&ctx_groestl);
               sph_groestl512 (&ctx_groestl, (&hash), 64);
               sph_groestl512_close(&ctx_groestl, (&hash));
               break;
          case 2:
               sph_jh512_init(&ctx_jh);
               sph_jh512 (&ctx_jh, (&hash), 64);
               sph_jh512_close(&ctx_jh, (&hash));
               break;
          case 3:
               sph_skein512_init(&ctx_skein);
               sph_skein512 (&ctx_skein, (&hash), 64);
               sph_skein512_close(&ctx_skein, static_cast<void*>(&hash));
               break;
        }
        if (fDebugHash) {
           printf("Hash R %d M %d : %s\n", round, method, hash.GetHex().c_str());
        }
    }

    return hash.trim256();

}



template<typename T1>
inline uint256 HashSHA3(const T1 pbegin, const unsigned int option) {

    char * ptr = (char*)(&pbegin[0]);

    if (fDebugHash) 
    {
       printf("Hash Input \n");
       for (unsigned int i = 0; i < 128; i++) {
           printf ("%.2x", (unsigned char)ptr[i]);
       }
       printf("\n");
    }

    switch (option & 0x000000ff) {
      case 7:
       return HashSHA3V1(ptr);
      case 8:
       return HashSHA3V2(ptr);
      case 10:
       return HashSHA3V3(ptr);
    }
    return HashSHA3V3POS(ptr);

}


#endif // HASHBLOCK_H