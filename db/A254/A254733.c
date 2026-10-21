/*
 * A254733  a(n) is the least k > n such that n divides k^3.
 * https://oeis.org/A254733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k > n such that n divides k³
   Computed by factoring n, forming the minimal base k₀
   that satisfies the divisibility condition, then taking the
   first multiple of k₀ that is greater than n. */
#include <stdint.h>   /* for uint64_t (thus idx_t if defined as such) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard against unused edge case */

    /* -------- factorize n ------------------------------------- */
    uint64_t tmp = (uint64_t)n;    /* use uint64_t for safe trial division */
    seq_t  base = (seq_t)1;        /* k₀, product of p^{ceil(e/3)} */

    /* trial division for prime factors up to sqrt(tmp) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            /* ceil(exp/3) = (exp+2)/3 */
            uint64_t kexp = (exp + 2) / 3;
            for (uint64_t i = 0; i < kexp; ++i)
                base *= (seq_t)p;
        }
    }
    /* remaining prime factor, if any */
    if (tmp > 1) {
        /* exponent 1 -> ceil(1/3)=1 */
        base *= (seq_t)tmp;
    }

    /* -------- find least multiple of base that exceeds n --------
       ans = ((n / base) + 1) * base
    */
    seq_t n_seq = (seq_t)n;
    seq_t ans = (n_seq / base + 1) * base;
    return ans;
}
