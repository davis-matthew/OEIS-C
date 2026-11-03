/* mulmod_seq: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_SEQ_H
#define OEISC_UTILS_MULMOD_SEQ_H
#include <stddef.h>
#include <stdint.h>

/* local multivalue mod function; uses 128‑bit arithmetic for 64‑bit types */
static inline seq_t mulmod_seq(seq_t x, seq_t y, seq_t mod) {
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 t = (unsigned __int128)x * (unsigned __int128)y;
        return (seq_t)(t % mod);
    } else {
        return (seq_t)((x * y) % mod);
    }
}

#endif
