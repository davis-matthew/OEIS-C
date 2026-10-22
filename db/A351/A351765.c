/*
 * A351765  a(n) = n! * Sum_{k=0..n} n^(n-k) * (n-k)^k/k!.
 * https://oeis.org/A351765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} n^(n-k)*(n-k)^k/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* running terms for the current k (k decreases from n to 0) */
    seq_t perm  = (seq_t)1;  /* n! / k! : starts with k = n  -> 1    */
    seq_t npow  = (seq_t)1;  /* n^(n-k) : starts with k = n  -> 1    */

    seq_t sum   = (seq_t)0;

    for (idx_t k = n; ; --k) {
        /* (n-k)^k */
        seq_t base      = (seq_t)(n - k);
        seq_t pow_base  = big_pow(base, (seq_t)k);

        /* term = (n! / k!) * n^(n-k) * (n-k)^k */
        seq_t term = perm;
        term *= npow;
        term *= pow_base;

        sum += term;

        if (k == 0)
            break;               /* finished all terms */

        /* update for next smaller k */
        perm *= (seq_t)k;          /* new perm = n!/(k-1)!            */
        npow *= (seq_t)n;          /* exponent of n increases by 1    */
    }

    return sum;
}
