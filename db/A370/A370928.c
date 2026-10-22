/*
 * A370928  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - x*exp(x^3))
 *          ).
 * https://oeis.org/A370928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Sum_{k=0..floor(n/3)} (n-3k)^k * (2n-3k)!/(k! *
 *            (n-3k)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow_seq.h"

static uint32_t *prime_array = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Build a list of all primes up to LIMIT (inclusive) */
static void ensure_primes(idx_t LIMIT)
{
    if (LIMIT <= prime_limit)
        return;

    /* Simple sieve */
    char *sieve = (char *)malloc(LIMIT + 1);
    if (!sieve)
        return;
    for (idx_t i = 0; i <= LIMIT; ++i)
        sieve[i] = 1;
    if (LIMIT >= 0) sieve[0] = 0;
    if (LIMIT >= 1) sieve[1] = 0;

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (sieve[p]) {
            for (idx_t j = p * p; j <= LIMIT; j += p)
                sieve[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            ++cnt;

    uint32_t *new_arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_arr) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i])
            new_arr[idx++] = (uint32_t)i;

    free(prime_array);
    prime_array = new_arr;
    prime_count = cnt;
    prime_limit = LIMIT;
    free(sieve);
}

/* Legendre exponent of prime p in N! */
static inline idx_t legendre(idx_t N, idx_t p)
{
    idx_t s = 0;
    while (N) {
        N /= p;
        s += N;
    }
    return s;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Ensure primes up to 2n */
    ensure_primes(2 * n);

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;                 /* n - 3k  */
        seq_t term = pow_seq((seq_t)m, (int)k);  /* (n-3k)^k */

        if (term == (seq_t)0)
            continue;  /* term will stay zero after multiplying primes */

        idx_t num = 2 * n - 3 * k;            /* 2n-3k   */

        /* Multiply by prime powers from the factorial quotient */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_array[i];
            if ((idx_t)p > num)
                break;

            idx_t e = legendre(num, (idx_t)p)
                    - legendre(k,   (idx_t)p)
                    - legendre(m,   (idx_t)p);

            if (e > 0) {
                seq_t p_pow = pow_seq((seq_t)p, (int)e);
                term = term * p_pow;
            }
        }

        sum += term;
    }

    return sum / ((seq_t)n + 1);
}
