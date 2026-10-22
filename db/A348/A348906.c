/*
 * A348906  Squares with a square number of 1's in their binary expansion.
 * https://oeis.org/A348906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 9648, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A348906: a(n) = the n-th perfect square whose binary representation
   contains a perfect‑square number of 1's.  Off‐set 1 (a(1)=0). */

#include <stdbool.h>
#include <stdint.h>

/* ----------------------------------------------------------------------
   Auxiliary helpers
   ---------------------------------------------------------------------- */

/* Count 1-bits of a value of type seq_t.
   Works for 8, 16, 32, 64, 128‑bit integers and for arbitrary‐precision
   integer classes that support 8‑bit masking and shifting by 8. */
static idx_t popcount_seq(seq_t x)
{
    if (sizeof(seq_t) <= 8) {
        /* 8‑byte or smaller: use the compiler built‑in. */
        return __builtin_popcountll((uint64_t)x);
    }
    /* Larger types: count 8‑bit chunks. */
    const seq_t mask = (seq_t)0xFFULL;   /* lower 8 bits */
    idx_t      cnt  = 0;
    seq_t      y    = x;
    while (y != (seq_t)0) {
        /* Count the lower 8 bits. */
        cnt += __builtin_popcountll((uint64_t)(y & mask));
        y >>= 8;                       /* shift by 8 bits */
    }
    return cnt;
}

/* Integer square root for idx_t values – returns ⌊√n⌋. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* mid*mid fits in seq_t because seq_t is at least as wide as idx_t. */
        seq_t prod = (seq_t)mid * (seq_t)mid;
        if (prod <= (seq_t)n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* ----------------------------------------------------------------------
   Main sequence function
   ---------------------------------------------------------------------- */

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)                /* safety for callers passing 0 */
        return (seq_t)0;

    idx_t found = 0;           /* number of valid squares found so far */

    /* Iterate over all integers i ≥ 0, examine i². */
    for (idx_t i = 0; ; ++i)
    {
        seq_t sq = (seq_t)i * (seq_t)i;          /* i² */

        /* Count 1-bits of the square. */
        idx_t ones = popcount_seq(sq);

        /* Check if ones is a perfect square. */
        idx_t r = isqrt_idx(ones);
        if (r * r == ones) {
            ++found;
            if (found == n)
                return sq;            /* nth term found */
        }
    }
}
