/*
 * A348974  Denominator of ratio A129283(n) / A003959(n), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and A129283(n) is sum of n
 *          and its arithmetic derivative.
 * https://oeis.org/A348974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A003959(n) / gcd(A003959(n), A129283(n)) where A003959 is
 *            multiplicative with a(p^e) = (p+1)^e and A129283(n) = n + D(n)
 *            with D(n) the arithmetic derivative
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include standard utilities needed for factorisation and gcd.  */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint64_t */

/*  Utilities provided by the framework.  */
#include "../utils/_floor_isqrt_u64.h"  /* _floor_isqrt_u64 */
#include "../utils/build_primes_upto.h" /* build_primes_upto */

/*  Primitive gcd for seq_t.  */
static seq_t
my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*  Cached prime list used for factorisation.  */
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static uint64_t  prime_limit = 0;

/*  Ensure we have at least 'limit' as the upper bound for prime sieving.  */
static void
ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    uint64_t *newp = NULL;
    size_t    newc = 0;
    build_primes_upto((idx_t)limit, &newp, &newc);
    free(primes);
    primes = newp;
    prime_cnt = newc;
    prime_limit = limit;
}

/*  Compute the arithmetic derivative D(n) as D(n) = n * Σ e/p for prime factorisation n = Π p^e.
    The result is returned as seq_t.  */
static seq_t
arith_derivative(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    uint64_t tmp = n;
    uint64_t sqrt_n = _floor_isqrt_u64(tmp);
    ensure_primes(sqrt_n);

    for (size_t i = 0; i < prime_cnt && primes[i] <= sqrt_n; ++i) {
        uint64_t p = primes[i];
        if (tmp % p != 0)
            continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        /* term = (n / p) * exp  */
        sum += ((seq_t)(n / p)) * exp;
        sqrt_n = _floor_isqrt_u64(tmp);
    }
    if (tmp > 1) /* remaining prime factor */
        sum += ((seq_t)(n / tmp)); /* exp == 1 */

    return sum;
}

/*  Compute A003959(n) = Π (p + 1)^e over prime factorisation.  */
static seq_t
A003959(uint64_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t product = 1;
    uint64_t tmp = n;
    uint64_t sqrt_n = _floor_isqrt_u64(tmp);
    ensure_primes(sqrt_n);

    for (size_t i = 0; i < prime_cnt && primes[i] <= sqrt_n; ++i) {
        uint64_t p = primes[i];
        if (tmp % p != 0)
            continue;
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        /* multiply by (p + 1)^exp */
        seq_t base = (seq_t)p + 1;
        seq_t pow  = 1;
        for (uint64_t j = 0; j < exp; ++j)
            pow *= base;
        product *= pow;
        sqrt_n = _floor_isqrt_u64(tmp);
    }
    if (tmp > 1) { /* remaining prime factor */
        seq_t base = (seq_t)tmp + 1;
        product *= base; /* exp == 1 */
    }

    return product;
}

/*  The main sequence function.  */
seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    seq_t a003959 = A003959(n64);
    seq_t deriv    = arith_derivative(n64);
    seq_t sum      = (seq_t)n64 + deriv;
    seq_t g        = my_gcd(a003959, sum);
    return a003959 / g;
}
