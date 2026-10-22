/*
 * A368072  Number of representations of n as the difference of two positive
 *          tetrahedral numbers.
 * https://oeis.org/A368072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of representations of n as the difference of two positive
   tetrahedral numbers.
   Tetrahedral number: T(k) = k(k+1)(k+2)/6.
   Count unordered pairs (a,b) with 1 <= b < a such that T(a)-T(b)=n. */

#include <stdint.h>

static inline seq_t tetra(idx_t k)
{
    /* Compute T(k) using 64‑bit arithmetic, then cast to seq_t. */
    idx_t v = k * (k + 1) * (k + 2) / 6;
    return (seq_t)v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sn = (seq_t)n;          /* target value as seq_t for comparison */
    for (idx_t a = 2; ; ++a)      /* start at 2 because b < a and b >= 1 */
    {
        seq_t Ta = tetra(a);
        seq_t minDiff = Ta - tetra(a - 1);   /* smallest difference for this a */
        if (minDiff > sn)          /* further a will only increase minDiff */
            break;

        for (idx_t b = 1; b < a; ++b)
        {
            seq_t diff = Ta - tetra(b);
            if (diff == sn)
                ++count;
            /* differences decrease as b increases; stop when below target */
            if (diff < sn)
                break;
        }
    }
    return count;
}
