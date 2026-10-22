/*
 * A359237  Number of divisors of 5*n-3 of form 5*k+1.
 * https://oeis.org/A359237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (5*n-3) that are ≡ 1 (mod 5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_div_mod1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_div_mod1.h"

/* Simple trial‐division factorisation for 64‑bit unsigned integers. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c]   = p;
            ex[c]   = e;
            ++c;
        }
    }
    if (n > 1) {            /* remaining prime factor */
        pr[c]   = n;
        ex[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1 – undefined for n=0 */
        return 0;

    uint64_t m64 = 5 * n - 3;    /* m = 5*n - 3 */

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t fact_cnt = 0;

    factor_u64(m64, primes, exponents, &fact_cnt);

    return count_div_mod1(primes, exponents, fact_cnt);
}
