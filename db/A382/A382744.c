/*
 * A382744  If k appears, 5*k does not.
 * https://oeis.org/A382744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose exponent of 5 in its
 *            prime factorization is even
 * Range:     u8 n <= 211, u16 n <= 8333, u32 n <= 8333, u64 n <= 8333,
 *            u128 n <= 8333, big any n
 * Helpers:   count_even_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_even_5.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 for safety */

    seq_t hi     = (seq_t)n * 6 / 5 + 5;  /* upper bound estimate */
    seq_t lo     = 1;
    seq_t target = (seq_t)n;

    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t cnt = count_even_5(mid);

        if (cnt >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;  /* the n-th term */
}
