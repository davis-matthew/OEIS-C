/*
 * A376021  a(n) = Sum_{d|n} d^(n/d - d) * binomial(n/d-1,d-1).
 * https://oeis.org/A376021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d - d) * binomial(n/d-1,d-1)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n≥1 for the sequence */

    /* Array for all divisors of n (max n elements is safe for n ≤ 10000) */
    idx_t *divs = (idx_t *)malloc(n * sizeof(idx_t));
    if (!divs) return 0;          /* allocation failure – unlikely in tests */

    size_t div_cnt = 0;
    divs[div_cnt++] = 1;          /* 1 is always a divisor */

    idx_t temp = n;
    /* factorise n and generate divisors on the fly */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            size_t cur = div_cnt;
            for (size_t i = 0; i < cur; ++i) {
                idx_t base = divs[i];
                idx_t val  = base;
                for (int k = 1; k <= cnt; ++k) {
                    val *= p;
                    divs[div_cnt++] = val;
                }
            }
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        size_t cur = div_cnt;
        for (size_t i = 0; i < cur; ++i) {
            divs[div_cnt++] = divs[i] * temp;
        }
    }

    seq_t result = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        idx_t m = n / d;
        if (m < d) continue;       /* skip when binomial would be zero (m-1 < d-1) */

        idx_t e = m - d;
        seq_t pow_val = big_pow((seq_t)d, (seq_t)e);
        seq_t bin_val = binomial(m - 1, d - 1);
        result += pow_val * bin_val;
    }

    free(divs);
    return result;
}
