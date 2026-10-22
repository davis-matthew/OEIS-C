/*
 * A332880  If n = Product (p_j^k_j) then a(n) = numerator of Product (1 +
 *          1/p_j).
 * https://oeis.org/A332880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of \prod_{p\mid n}\!\left(1+\frac1p\right) (the
 *            product is over the distinct prime divisors of n)
 * Range:     u8 n <= 200, u16 n <= 9485, u32 n <= 9485, u64 n <= 9485,
 *            u128 n <= 9485, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

/* primes cache up to the required square‑root bound */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t  primes_limit = 0;

/* make sure primes[] contains all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;
    if (primes) free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* offset handling: a(1)=1; undefined for n=0 but harmless */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* integer square‑root of n */
    idx_t sqrt_n = _floor_isqrt_u64(n);
    if (sqrt_n < 2) sqrt_n = 2;          /* at least 2 for prime tests */

    ensure_primes(sqrt_n);

    seq_t prod_num = 1;   /* numerator = ∏(p+1) */
    seq_t prod_den = 1;   /* denominator = ∏p       */

    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_n; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            prod_num *= (seq_t)(p + 1);
            prod_den *= (seq_t)p;
            while (temp % p == 0) temp /= p;   /* remove all powers of p */
        }
    }
    if (temp > 1) { /* remaining prime factor > sqrt(n) */
        prod_num *= (seq_t)(temp + 1);
        prod_den *= (seq_t)temp;
    }

    seq_t g = gcd(prod_num, prod_den);
    return prod_num / g;
}
