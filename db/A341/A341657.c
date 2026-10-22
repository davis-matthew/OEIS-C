/*
 * A341657  a(n) is the number of divisors of prime(n)^6 - 1.
 * https://oeis.org/A341657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ( prime(n)^6 - 1 ), a(1)=6, a(2)=16, …
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisor count of p⁶–1 where p is the n‑th prime (1‑based). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) {                /* n is 1‑based, 0 is undefined */
        return 0;
    }

    /* the n-th prime (1‑based) */
    uint64_t p = get_prime(n);

    /* the four coprime factors of p⁶–1 */
    uint64_t f[4];
    f[0] = p - 1ULL;            /* p‑1 */
    f[1] = p + 1ULL;            /* p+1 */
    f[2] = p * p - p + 1ULL;    /* p²–p+1 */
    f[3] = p * p + p + 1ULL;    /* p²+p+1 */

    /* merged prime exponents of the product of the four factors */
    uint64_t merged_primes[32];
    uint64_t merged_exp[32];
    size_t merged_cnt = 0;

    /* temporary arrays for one factor */
    uint64_t primes_buf[32];
    uint64_t exps_buf[32];
    size_t cnt;

    for (int i = 0; i < 4; ++i) {
        uint64_t x = f[i];
        if (x <= 1) {
            continue;           /* skip trivial factor 1 */
        }

        /* factor this 64‑bit integer */
        cnt = 0;
        factor_u64(x, primes_buf, exps_buf, &cnt);

        /* merge the prime powers */
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t pr = primes_buf[j];
            uint64_t ex = exps_buf[j];

            size_t k = 0;
            while (k < merged_cnt && merged_primes[k] != pr) {
                ++k;
            }
            if (k == merged_cnt) {
                merged_primes[merged_cnt] = pr;
                merged_exp[merged_cnt] = ex;
                ++merged_cnt;
            } else {
                merged_exp[k] += ex;
            }
        }
    }

    /* compute divisor count = product of (exponent+1) */
    seq_t result = 1;
    for (size_t i = 0; i < merged_cnt; ++i) {
        result = result * ((seq_t)merged_exp[i] + 1);
    }
    return result;
}
