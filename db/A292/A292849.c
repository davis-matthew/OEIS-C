/*
 * A292849  a(n) is the least positive k such that the Hamming weight of k
 *          equals the Hamming weight of k*n.
 * https://oeis.org/A292849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A292849:
   a(n) = the least positive k such that popcount(k) == popcount(k*n) */
#include <stdint.h>

/* Bit‑count of an unsigned integer that may be any width. */
static inline int popcount_seq_t(seq_t x)
{
    int c = 0;
    while (x)
    {
        c += (int)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Remove all factors of 2:  a(2^m * n) = a(n) for m >= 0 */
    idx_t t = n;
    while ((t & 1) == 0)
        t >>= 1;
    seq_t nn = (seq_t)t;   /* use the reduced n for multiplication */

    seq_t k = 1;           /* start checking from k = 1 */
    while (1)
    {
        int pc_k   = popcount_seq_t(k);
        seq_t prod = k * nn;
        int pc_prod = popcount_seq_t(prod);

        if (pc_k == pc_prod)
            return k;     /* smallest k found */
        k++;
    }
}
