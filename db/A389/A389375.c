/*
 * A389375  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(n-k-1,n-3*k).
 * https://oeis.org/A389375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *            binomial(n-k-1,n-3*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid negative arguments in binomial */
    if (n == 0) return 1;
    if (n == 1 || n == 2) return 0;

    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t t1 = binomial(n + k - 1, k);          /* binomial(n+k-1, k) */
        seq_t t2 = binomial(n - k - 1, n - 3 * k);  /* binomial(n-k-1, n-3k) */
        sum += t1 * t2;
    }
    return sum;
}
