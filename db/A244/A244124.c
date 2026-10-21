/*
 * A244124  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of 2^n-1 as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, k = n -
 *            r(r+1)/2, T(r,0)=0; for k>0, T(r,k) = (1-k)^(k-1)*(1+k)^(r-k)
 * Range:     i8 n <= 18, i16 n <= 34, i32 n <= 75, i64 n <= 169,
 *            i128 n <= 432, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* find r such that r(r+1)/2 <= n */
    idx_t low = 0, high = 1;
    while (((unsigned __int128)high * (high + 1ULL) / 2) <= n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((unsigned __int128)mid * (mid + 1ULL) / 2) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1ULL)) / 2);
    idx_t k = n - start;
    if (k == 0) {
        return (seq_t)0;
    }
    seq_t base1 = (seq_t)1 - (seq_t)k;
    seq_t base2 = (seq_t)1 + (seq_t)k;
    seq_t pow1 = big_pow(base1, (seq_t)(k - 1));
    seq_t pow2 = big_pow(base2, (seq_t)(r - k));
    return pow1 * pow2;
}
