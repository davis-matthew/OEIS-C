/*
 * A376553  Largest unitary square divisor of binomial(n, floor(n/2)).
 * https://oeis.org/A376553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 129, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A376553 – Largest unitary square divisor of C(n,⌊n/2⌋)
 *
 *  Formula:  a(n) = ∏p p^e  where e = v_p(C(n,⌊n/2⌋)) and e is even.
 *
 *  The exponent v_p(C(n,k)) is computed by Legendre’s formula:
 *      v_p(C(n,k)) = v_p(n!) – v_p(k!) – v_p((n‑k)!)
 *
 *  For every prime p ≤ n the exponent e is obtained by repeated division.
 *  Only primes with even positive exponent contribute to the product.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"      /* big_pow(seq_t, seq_t) */
#include "../utils/build_primes.h" /* build_primes(idx_t, idx_t**, size_t*) */

static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static idx_t prime_max = 0;

/* Ensure that prime_cache contains all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (prime_max >= limit)
        return;

    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes(limit, &new_primes, &new_cnt);

    /* Replace old cache */
    if (prime_cache != NULL)
        free(prime_cache);
    prime_cache = new_primes;
    prime_cnt  = new_cnt;
    prime_max  = limit;
}

/* v_p(n!) – exponent of prime p in n! */
static idx_t v_p_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n)
    {
        n /= p;
        res += n;
    }
    return res;
}

/* Largest unitary square divisor of C(n, floor(n/2)) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 2;           /* floor(n/2) */
    idx_t nk = n - k;          /* n - k */

    /* Prepare primes up to n */
    ensure_primes(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_cache[i];
        if (p > n)
            break;

        /* exponent of p in C(n,k) */
        idx_t exp = v_p_factorial(n, p)
                  - v_p_factorial(k, p)
                  - v_p_factorial(nk, p);

        /* Include only even positive exponents */
        if ((exp & 1u) == 0 && exp > 0)
        {
            seq_t part = big_pow((seq_t)p, (seq_t)exp);
            result *= part;
        }
    }

    return result;
}
