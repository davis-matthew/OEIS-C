/*
 * A272760  a(n) = A065880(n)/n.
 * https://oeis.org/A272760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{i : popcount(n * i) = i}; if no such i, a(n) = 0
 *            Computation: iterate i = 0,1,2,… and compute popcount of n*i.
 *            Since popcount(n*i) ≤ bitlen(n*i) < i eventually holds, the loop
 *            terminates after a few dozen iterations
 * Range:     u8 n <= 52, u16 n <= 6934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t best = 0;
    for (idx_t i = 0; ; ++i)
    {
        /* product and popcount */
        seq_t prod = (seq_t)n * (seq_t)i;
        seq_t pc = 0;
        seq_t tmp = prod;
        while (tmp)
        {
            pc += tmp & 1;
            tmp >>= 1;
        }
        if (pc == (seq_t)i)
            best = i;

        /* bit length of prod */
        seq_t bl = 0;
        tmp = prod;
        while (tmp)
        {
            tmp >>= 1;
            ++bl;
        }
        if (bl < i)   /* further i cannot satisfy popcount(i) = i */
            break;
    }
    return best;
}
