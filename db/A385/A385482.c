/*
 * A385482  a(n) is the least number k such that k*n is a binary Niven number
 *          (A049445).
 * https://oeis.org/A385482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = smallest k ≥ 1 such that k*n is a binary Niven number, *
 *            i.e., (k*n) % popcount(k*n) == 0
 * Range:     u8 n <= 24, u16 n <= 6808, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x)
    {
        x &= (x - 1);
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    for (idx_t k = 1;; ++k)
    {
        seq_t m = (seq_t)k * (seq_t)n;
        seq_t pc = popcount_seq(m);
        if (m % pc == 0)
            return (seq_t)k;
    }
}
