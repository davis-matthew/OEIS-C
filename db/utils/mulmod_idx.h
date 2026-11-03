/* mulmod_idx: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_IDX_H
#define OEISC_UTILS_MULMOD_IDX_H
#include <stdint.h>

/*  128‑bit modular multiplication for idx_t values if available.
    Falls back to 64‑bit multiplication otherwise (works for the
    sizes needed in this sequence).
*/
static inline idx_t mulmod_idx(idx_t a, idx_t b, idx_t mod)
{
#ifdef __SIZEOF_INT128__
    return (idx_t)(((__int128)a * b) % mod);
#else
    return (idx_t)((a * b) % mod);
#endif
}

#endif
