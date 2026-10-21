/*
 * A255683  Sum of the binary numbers whose digits are cyclic permutations of
 *          the binary expansion of n.
 * https://oeis.org/A255683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of all cyclic rotations of the binary representation
 *            of n (without leading zeros) interpreted as binary numbers
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute bit length L of n */
    unsigned long long val = (unsigned long long)n;
    int L = 64 - __builtin_clzll(val);  /* n >= 1 */

    /* compute mask of L bits: (1 << L) - 1, handling L == 64 */
    seq_t mask;
    if (L == 64)
        mask = ~((seq_t)0);
    else
        mask = (((seq_t)1 << L) - 1);

    seq_t sum = 0;
    for (int k = 0; k < L; ++k)
    {
        seq_t rot;
        if (k == 0)
            rot = (seq_t)n;
        else
            rot = ((((seq_t)n >> k) | ((seq_t)n << (L - k))) & mask);
        sum += rot;
    }
    return sum;
}
