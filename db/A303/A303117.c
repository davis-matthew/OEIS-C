/*
 * A303117  a(n) is the number of cyclic permutations with at most two
 *          descents.
 * https://oeis.org/A303117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(3,n) - n * L(2,n) where L(k,n) = (1/n) * Σ_{d|n} μ(d) *
 *            k^{n/d}
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static inline int mobius(idx_t n) {
    if (n == 1) return 1;
    int cnt = 0;
    idx_t x = n;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            ++cnt;
            x /= p;
            if (x % p == 0) return 0;        /* square factor */
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;             /* offset */

    seq_t sum1 = 0;                          /* Σ μ(d) · 3^{n/d} */
    seq_t sum2 = 0;                          /* Σ μ(d) · 2^{n/d} */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d) continue;

        int mu1 = mobius(d);
        if (mu1 != 0) {
            idx_t exp = n / d;
            seq_t p3 = big_pow((seq_t)3, (seq_t)exp);
            seq_t p2 = big_pow((seq_t)2, (seq_t)exp);
            if (mu1 > 0) {
                sum1 += p3;
                sum2 += p2;
            } else {
                sum1 -= p3;
                sum2 -= p2;
            }
        }

        idx_t d2 = n / d;
        if (d2 == d) continue;              /* avoid duplicate (square root) */

        int mu2 = mobius(d2);
        if (mu2 != 0) {
            idx_t exp = n / d2;              /* equals d */
            seq_t p3 = big_pow((seq_t)3, (seq_t)exp);
            seq_t p2 = big_pow((seq_t)2, (seq_t)exp);
            if (mu2 > 0) {
                sum1 += p3;
                sum2 += p2;
            } else {
                sum1 -= p3;
                sum2 -= p2;
            }
        }
    }

    seq_t L3 = sum1 / (seq_t)n;             /* L(3,n) */
    seq_t result = L3 - sum2;               /* a(n) = L3 - n*L2 = L3 - sum2 */
    return result;
}
