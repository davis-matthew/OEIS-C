/*
 * A397838  Antidiagonal products of the multifactorial array.
 * https://oeis.org/A397838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} ∏_{m=1..⌊n/k⌋} (n + 1 – m*k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses divisor‑counts to factorise the product efficiently. */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return 1;
    }

    /* divisor counts τ(1)…τ(n) */
    idx_t *tau = (idx_t *)calloc(n + 1, sizeof(idx_t));
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            tau[j]++;
        }
    }

    /* smallest prime factor array */
    idx_t *spf = NULL;
    build_spf(n, &spf);          /* spf[0] unused, spf[1] = 1 */

    /* prime exponents in the final product */
    uint64_t *prime_exp = (uint64_t *)calloc(n + 1, sizeof(uint64_t));

    /* accumulate exponents: a(n) = ∏_{t=1..n} (n+1 - t)^{τ(t)} */
    for (idx_t t = 1; t <= n; ++t) {
        idx_t val = n + 1 - t;
        idx_t tmp = val;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                cnt++;
            }
            prime_exp[p] += (uint64_t)tau[t] * cnt;
        }
    }

    seq_t result = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (prime_exp[p]) {
            seq_t term = big_pow((seq_t)p, (seq_t)prime_exp[p]);
            result *= term;
        }
    }

    free(tau);
    free(spf);
    free(prime_exp);
    return result;
}
