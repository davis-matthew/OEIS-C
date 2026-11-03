/* powmod: b^e mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POWMOD_H
#define OEISC_UTILS_POWMOD_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t powmod(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
}

#endif
