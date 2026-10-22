/*
 * A394905  Smallest number k > n such that the binary Hamming distance
 *          between n and k is equal to the number of runs in the binary
 *          representation of n, and k has more runs than n.
 * https://oeis.org/A394905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { k > n : d_H(n, k) = R(n) and R(k) > R(n) }
 * Range:     u8 n <= 84, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Count the number of runs in the binary representation of x.
   Runs are maximal groups of identical consecutive bits, ignoring
   leading zeros. */
static idx_t run_count(seq_t x)
{
    if (x == (seq_t)0)
        return 1;                /* not used for n>=1, but safe */

    idx_t count = 0;
    seq_t prev    = x & (seq_t)1;   /* least significant bit */
    count = 1;

    idx_t len = bitlen_seq_t(x);
    for (idx_t i = 1; i < len; ++i)
    {
        seq_t b = (x >> i) & (seq_t)1;
        if (b != prev)
        {
            ++count;
            prev = b;
        }
    }
    return count;
}

/* Popcount for seq_t values, generic for bits up to 128 or arbitrary
   precision, using the classic x &= x-1 loop. */
static idx_t popcount(seq_t x)
{
    idx_t c = 0;
    while (x)
    {
        x &= x - (seq_t)1;
        ++c;
    }
    return c;
}

/* Sequence A394905: smallest k > n with Hamming distance equal to
   R(n) and > R(n) runs. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t once for efficiency */
    seq_t sn = (seq_t)n;
    idx_t runs_n = run_count(sn);

    seq_t k = sn + (seq_t)1;          /* start search at n+1 */

    while (1)
    {
        seq_t diff = sn ^ k;
        idx_t hd = popcount(diff);
        if (hd == runs_n)
        {
            idx_t runs_k = run_count(k);
            if (runs_k > runs_n)
                return k;
        }
        ++k;
    }
    /* unreachable */
}
