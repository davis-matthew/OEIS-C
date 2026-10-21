/*
 * A257250  Numbers n for which A256999(n) = n; numbers that cannot be made
 *          any larger by rotating (by one or more steps) the non-msb bits of
 *          their binary representation (with A080541 or A080542).
 * https://oeis.org/A257250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 4808, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257250 – numbers that cannot be increased by rotating the
 * non‑MSB bits of their binary representation.
 * The routine builds the sequence on demand by exhaustive search
 * of rotation maximality.  It works for any unsigned seq_t
 * (uint8_t … uint128_t) and idempotently caches previous terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* compute floor(log2(v)) for a 64‑bit unsigned integer (v > 0) */
static inline unsigned floor_log2_idx(idx_t v)
{
    return 63 - __builtin_clzll((uint64_t)v);
}

/* return true iff the bits following the most significant '1'
 * of m form a lexicographically maximum rotation. */
static inline bool suffix_is_maximal(idx_t m)
{
    /* length of the suffix (number of bits after the msb) */
    unsigned len = floor_log2_idx(m);           /* 0 ≤ len ≤ 63 */

    /* Empty suffix (m == 1) is trivially maximal. */
    if (len == 0) return true;

    /* mask to extract the suffix bits */
    uint64_t mask   = ((uint64_t)1 << len) - 1;
    uint64_t suffix = m & mask;

    for (unsigned k = 1; k < len; ++k) {
        unsigned __int128 shifted = (unsigned __int128)suffix << k;
        unsigned __int128 rot128  = shifted | ((unsigned __int128)suffix >> (len - k));
        uint64_t rot = (uint64_t)(rot128 & mask);
        if (rot > suffix) return false;         /* found a larger rotation */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Cached generation of the sequence.                              */
/* ------------------------------------------------------------------ */
static seq_t *seq_arr  = NULL;   /* array of stored terms          */
static size_t  seq_cap = 0;      /* current capacity of seq_arr    */
static size_t  seq_len = 0;      /* number of terms stored so far  */
static idx_t   next_candidate = 1; /* next integer to test          */

/* ensure that seq_arr contains at least (req+1) terms (index req) */
static void ensure_terms(idx_t req)
{
    if (seq_len == 0) {           /* first initialisation          */
        seq_cap     = 64;
        seq_arr     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0]  = (seq_t)0;  /* a(0) = 0                       */
        seq_len     = 1;
        next_candidate = 1;
    }

    while (seq_len <= (size_t)req) {
        idx_t cand = next_candidate++;
        if (!suffix_is_maximal(cand)) continue;   /* not a member */

        if (seq_len >= seq_cap) {                 /* grow array   */
            seq_cap <<= 1;
            seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
        }
        seq_arr[seq_len++] = (seq_t)cand;          /* store term   */
    }
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq_arr[n];
}
