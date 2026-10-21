/*
 * A272501  Number of n-letter strings over a ten letter alphabet where no
 *          letter appears exactly three times.
 * https://oeis.org/A272501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{min(10,⌊n/3⌋)} (-1)^k · C(10,k) · n!/(n-3k)! /
 *            6^k · (10−k)^(n-3k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, pow2, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inclusion–exclusion over the 10 letters. */

#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static const seq_t C10[11] = {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1};

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= 10; ++k) {
        if (n < 3 * k) break;          /* further k's cannot satisfy 3k ≤ n */

        /* coefficient C(10,k) */
        seq_t term = C10[k];

        /* compute  n!/(n-3k)! / 6^k  */
        seq_t p = 1;
        int count2 = 0, count3 = 0;
        for (idx_t i = n - 3 * k + 1; i <= n; ++i) {
            idx_t t = i;
            while ((t & 1ULL) == 0) { t >>= 1; ++count2; }          /* factor 2 */
            while (t % 3ULL == 0) { t /= 3; ++count3; }            /* factor 3 */
            p = seq_mul(p, (seq_t)t);
        }

        /* remove the 6^k part by multiplying back the surplus 2^x * 3^y */
        if (count2 > k) p = seq_mul(p, pow2((idx_t)(count2 - k)));
        if (count3 > k) p = seq_mul(p, pow3((idx_t)(count3 - k)));

        /* multiply by (10-k)^(n-3k) */
        seq_t pow_val = big_pow((seq_t)(10 - k), (seq_t)(n - 3 * k));
        p = seq_mul(p, pow_val);

        term = seq_mul(term, p);

        /* add or subtract depending on parity of k */
        if (k & 1) {
            /* subtract: result ≥ term for all n, so unsigned underflow is safe */
            result = result - term;
        } else {
            result = result + term;
        }
    }
    return result;
}
