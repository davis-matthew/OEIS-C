/*
 * A346510  a(n) is the number of nontrivial divisors of A346507(n) ending
 *          with 1.
 * https://oeis.org/A346510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A346392(A346507(n)) - 1; computed by search: find the
 *            n-th k ending in 1 that is a product of two factors >1 ending in
 *            1, then count its divisors d, 1<d<k, with d ending in 1
 * Range:     u8 n <= 3, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A346510_kth(idx_t n)
{
    seq_t candidate = 11;
    idx_t count = 0;
    while (1)
    {
        for (seq_t d = 11; d <= candidate / d; d += 10)
        {
            if (candidate % d == 0)
            {
                ++count;
                if (count == n) return candidate;
                break;
            }
        }
        candidate += 10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t m = A346510_kth(n);
    seq_t cnt = 0;
    for (seq_t d = 11; d <= m / d; d += 10)
    {
        if (m % d == 0)
        {
            ++cnt;
            seq_t e = m / d;
            if (e != d) ++cnt;
        }
    }
    return cnt;
}
