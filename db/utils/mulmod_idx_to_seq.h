/* mulmod_idx_to_seq: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_IDX_TO_SEQ_H
#define OEISC_UTILS_MULMOD_IDX_TO_SEQ_H
#include <stdbool.h>
#include <stdint.h>

static inline seq_t mulmod_idx_to_seq(seq_t a, idx_t b, seq_t mod)
{
    /* multiply a by b modulo mod using 128‑bit intermediate to avoid overflow */
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b % mod);
}

#endif
