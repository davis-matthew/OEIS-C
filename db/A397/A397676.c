/*
 * A397676  Sum of absolute differences of consecutive terms of n/gcd(n,k), k
 *          = 1..n-1.
 * https://oeis.org/A397676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Σ_{k=1}^{n-2} | n/gcd(n,k+1) - n/gcd(n,k) |
 * Range:     u8 n <= 23, u16 n <= 327, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A397676 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2) {
        /* For n = 1 or 2 the sum is over an empty range */
        return (seq_t)0;
    }

    seq_t nseq = (seq_t)n;
    /* First term (k = 1) */
    seq_t prev = nseq / gcd(nseq, (seq_t)1);

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n - 2; ++k) {
        seq_t cur = nseq / gcd(nseq, (seq_t)(k + 1));
        seq_t diff = (prev > cur) ? (prev - cur) : (cur - prev);
        sum += diff;
        prev = cur;
    }

    return sum;
}
