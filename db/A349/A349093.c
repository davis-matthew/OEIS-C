/*
 * A349093  a(n) is the smallest nonprime number m (m = A018252(t)) such that
 *          n divides the product P(t) of all nonprime numbers up to and
 *          including m (P(t) = A036691(t-1)).
 * https://oeis.org/A349093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by factoring n and for each prime power p^k
 *            compute the * minimal w≥2 with v_p((w*p)!)≥k+1 (Legendre
 *            exponent), then a(n)=max(w*p) * over all prime powers; special
 *            case n=1 returns 1
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>   /* for sqrt, but we will use int_sqrt from utils */
#include <stdbool.h>

#include "../utils/int_sqrt.h"   /* int_sqrt(idx_t) */
#include "../utils/build_primes_upto.h"  /* build_primes_upto(idx_t, uint64_t **, size_t *) */

/* static holders for primes */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_limit = 0;

/* Ensure that primes up to 'limit' are available */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Compute v_p((m*p)!) using Legendre's formula */
static uint64_t vp_factorial(uint64_t p, uint64_t m)
{
    uint64_t v = 0;
    uint64_t div = p;
    uint64_t n = m * p;
    while (div <= n) {
        v += n / div;
        if (div > n / p) break;   /* prevent overflow */
        div *= p;
    }
    return v;
}

/* Find minimal w >= 2 such that v_p((w*p)!) >= k+1 */
static seq_t minimal_w(uint64_t p, uint64_t k)
{
    uint64_t target = k + 1;   /* we need v >= k+1 */
    uint64_t w = 2;
    while (1) {
        uint64_t val = vp_factorial(p, w);
        if (val >= target) break;
        ++w;
    }
    return (seq_t)(w * p);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* special case */

    /* factor n */
    uint64_t nn = (uint64_t)n;
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes(sqrt_n);

    seq_t result = 0;    /* will store maximum value */

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_n; ++i) {
        uint64_t p = prime_list[i];
        if (nn % p == 0) {
            uint64_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            seq_t val = minimal_w(p, exp);
            if (val > result) result = val;
        }
    }

    /* If something remains >1, it's a prime factor with exponent 1 */
    if (nn > 1) {
        seq_t val = minimal_w((uint64_t)nn, 1);
        if (val > result) result = val;
    }

    return result;
}
