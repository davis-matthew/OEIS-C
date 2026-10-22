/*
 * A396686  a(n) = floor(2*log_2(n)) + 2.
 * https://oeis.org/A396686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*log_2(n)) + 2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ⌊log₂(v)⌋ for a 64‑bit integer (v ≥ 0). */
static inline idx_t floor_log2_uint64(idx_t v)
{
    return v == 0 ? 0U : 63ULL - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; guard against n == 0. */
    if (n == 0) return 0;  /* undefined for the offset 1, but safe */

    idx_t k = floor_log2_uint64(n);   /* ⌊log₂(n)⌋ */
    idx_t bits = 2 * k;               /* 2·⌊log₂(n)⌋ */

    /* 128‑bit arithmetic to avoid overflow when n is large. */
    unsigned __int128 x  = (unsigned __int128)n;
    unsigned __int128 x2 = x * x;

    unsigned int shift  = (unsigned int)(bits + 1);               /* 1 ≤ shift ≤ 127 */
    unsigned __int128 lim = (unsigned __int128)1 << shift;      /* 2^(bits+1) */

    idx_t floor_log2_n2 = (x2 < lim) ? bits : bits + 1;           /* ⌊log₂(n²)⌋ */

    return (seq_t)floor_log2_n2 + (seq_t)2;
}
