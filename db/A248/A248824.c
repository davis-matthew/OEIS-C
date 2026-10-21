/*
 * A248824  Number of integers k^6 that divide 1!*2!*3!*...*n!.
 * https://oeis.org/A248824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{p≤n} (⌊E_p/6⌋+1), where E_p = ∑_{i=1}^{n} ∑_{k≥1}
 *            ⌊i/p^k⌋ is the exponent of p in ∏_{i=1}^{n} i!
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 63, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* generate all primes ≤ n (simple trial division, n ≤ 10000) */
    size_t max_n = (size_t)n;
    uint32_t *prime_list = (uint32_t *)malloc((max_n + 1) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        int is_prime = 1;
        for (idx_t d = 2; d * d <= i; ++d) {
            if (i % d == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            prime_list[primes_cnt++] = (uint32_t)i;
    }

    seq_t ans = 1;

    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = prime_list[pi];
        seq_t exp = 0;
        idx_t pow = p;

        while (pow <= n) {
            /* t = floor(n / pow) */
            idx_t t = n / pow;

            /* sum_{i=1}^{n} ⌊i/pow⌋ = pow * t*(t-1)/2 + (n - pow*t + 1)*t */
            seq_t part1 = (seq_t)pow * t * (t - 1) / 2;
            seq_t part2 = ((seq_t)(n - pow * t + 1)) * t;
            exp += part1 + part2;

            pow *= p;
        }

        seq_t factor = exp / 6 + 1;
        ans *= factor;
    }

    free(prime_list);
    return ans;
}
