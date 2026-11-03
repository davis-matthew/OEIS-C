/* addmod: (a+b) mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ADDMOD_H
#define OEISC_UTILS_ADDMOD_H
#include <stdbool.h>
#include <stdint.h>

/* ---------- small modular arithmetic helpers ---------- */
static inline seq_t addmod(seq_t a, seq_t b, seq_t mod)
{
    uint64_t sum = ((uint64_t)a + (uint64_t)b) % (uint64_t)mod;
    return (seq_t)sum;
}

#endif
