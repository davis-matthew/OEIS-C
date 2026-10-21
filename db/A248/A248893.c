/*
 * A248893  a(0)=0; for n>0, choose a(n) to be the smallest number > a(n-1)
 *          such that the condition a(n) > Sum_{k=0..n} (number of 1's in
 *          binary expansion of n a(k)) holds.
 * https://oeis.org/A248893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; for n>0, a(n) is the smallest integer > a(n-1) such that
 *            a(n) > sum_{k=0}^{n-1} popcount(a(k)) + popcount(a(n)) (popcount
 *            = number of 1’s in the binary representation)
 * Range:     u8 n <= 85, u16 n <= 9905, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count the 1‑bits in an unsigned integer of type seq_t. */
static seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

/* Sequence a(n).  The cache grows as needed and works for calls in
   arbitrary order. */
seq_t a(idx_t n)
{
    /* static state: the computed terms and the sum of their popcounts */
    static seq_t *seq = NULL;       /* array of a(0)..a(len-1) */
    static idx_t len = 0;           /* number of terms currently stored */
    static seq_t sum_pop = 0;       /* Σ popcount(a(k)), k from 0 to len-1 */

    /* Initialize with a(0)=0 */
    if (len == 0)
    {
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = (seq_t)0;
        len = 1;
        sum_pop = 0;                /* popcount(0)=0 */
    }

    /* Compute missing terms up to index n */
    if (n + 1 > len)
    {
        /* Expand storage */
        seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));

        for (idx_t i = len; i <= n; ++i)
        {
            seq_t prev = seq[i - 1];
            seq_t candidate = prev + 1;
            seq_t threshold = sum_pop;   /* Σ popcount(a(k)), k < i */

            /* Find the smallest candidate satisfying the definition */
            while (1)
            {
                seq_t pc = popcount_seq(candidate);
                if (candidate > threshold + pc)
                {
                    seq[i] = candidate;
                    sum_pop += pc;        /* update cumulative popcount */
                    break;
                }
                ++candidate;
            }
        }
        len = n + 1;
    }

    return seq[n];
}
