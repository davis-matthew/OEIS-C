/*
 * A380763  List triangular numbers up to increasingly large values and back
 *          down to 0.
 * https://oeis.org/A380763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A380763: list triangular numbers up to m and back down to 1.
    a(n) = T(k), where k is determined by the block index m that contains
    n.  The block m (m ≥ 1) has length 2m and consists of
          0, 1, 3, …, T(m), T(m‑1), …, 1
    where T(m) = m*(m+1)/2.  The cumulative length of the first m blocks
    is m*(m+1).  For a given n, find the smallest m such that
           m*(m+1) > n
    Then let p = n – (m‑1)*m (0‑based position inside the block).
    If p ≤ m  → a(n) = T(p).
    Otherwise  a(n) = T(2*m – p).
    All arithmetic is done with unsigned integer types; no floating
    point or external math library is used. */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x)
{
    /* floor(sqrt(x)) using a bit‑shift algorithm */
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;           /* highest possible bit */
    while (bit)
    {
        idx_t tmp = r + bit;
        if (tmp <= x / tmp)
            r = tmp;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Compute m = smallest integer with m*(m+1) > n  */
    idx_t sqrtv = isqrt_u64((idx_t)4 * n + 1);
    idx_t m = (sqrtv - 1) / 2 + 1;            /* block index, m ≥ 1 */

    idx_t offset = (m - 1) * m;              /* starting index of block m */
    idx_t p = n - offset;                    /* position inside block */

    if (p <= m)                              /* ascending part */
    {
        seq_t k = (seq_t)p;
        return k * (k + 1) / 2;
    }
    else                                     /* descending part */
    {
        seq_t k = (seq_t)(2 * m - p);
        return k * (k + 1) / 2;
    }
}
