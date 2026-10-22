/*
 * A362197  Number of Grassmannian permutations of size n that avoid a
 *          pattern, sigma, where sigma is a pattern of size 10 with exactly
 *          one descent.
 * https://oeis.org/A362197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{i=2..9} binomial(n,i)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 51, u64 n <= 576,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;       /* use symmetry, k is at most 9 */
    seq_t num[9];
    seq_t den[9];
    for (idx_t i = 0; i < k; ++i) {
        num[i] = (seq_t)(n - i);
        den[i] = (seq_t)(i + 1);
    }
    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = 0; j < k; ++j) {
            if (den[j] == (seq_t)1) continue;
            seq_t g = gcd(num[i], den[j]);
            if (g != (seq_t)1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        res *= num[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;
    seq_t sum = (seq_t)1;
    for (idx_t k = 2; k <= 9; ++k) {
        if (n < k) break;
        sum += binom_small(n, k);
    }
    return sum;
}
