/*
 * A348301  a(n) is the difference between the numerator and denominator of
 *          the (reduced) fraction Sum_{i = 1..n} 1/prime(i).
 * https://oeis.org/A348301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{i=1..n} 1/p_i reduced numerator minus denominator
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = -1, a(n) = p_n*a(n-1) + primorial(n-1). */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* maximum number of terms to be supported (≈ 10000 needed) */
#define MAX_N 20000
/* sieve upper bound sufficient to contain MAX_N primes (≈ 224k for 20000 primes) */
#define PRIME_LIMIT 300000

/* storage for first MAX_N primes */
static uint32_t primes[MAX_N];
static size_t primes_cnt = 0;
static bool primes_initialized = false;
/* sieve mask used during initialization */
static bool sieve_mask[PRIME_LIMIT + 1];

/* cache of already computed a(n) values */
static seq_t cache[MAX_N + 1];
static size_t cache_computed_up_to = 0;
/* previous primorial and a value used for recurrence */
static seq_t Q_prev = 0;   /* primorial of last computed index */
static seq_t a_prev = 0;   /* a(n-1) */

/* ------------------------------------------------------------------ */
/*  generate first MAX_N primes using a simple sieve of Eratosthenes  */
static void
init_primes(void)
{
    if (primes_initialized)
        return;

    /* initialise sieve mask */
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        sieve_mask[i] = false;

    size_t count = 0;
    for (int i = 2; i <= PRIME_LIMIT && count < MAX_N; ++i) {
        if (!sieve_mask[i]) {
            primes[count++] = (uint32_t)i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve_mask[j] = true;
            }
        }
    }
    primes_cnt = count;
    primes_initialized = true;
    assert(primes_cnt >= 1);              /* at least 2 available */
    assert((size_t)MAX_N >= primes_cnt); /* so we support all needed n */
}

/* ------------------------------------------------------------------ */
/*  Main function required by the judge                                  */
seq_t
a(idx_t n)
{
    /* input must be within the supported range */
    assert(n >= 1);
    assert((idx_t)MAX_N >= n);

    init_primes();
    assert((size_t)n <= primes_cnt);

    /* initialise cache on first call */
    if (cache_computed_up_to == 0) {
        cache[1] = (seq_t)-1;            /* a(1) = -1 */
        Q_prev   = (seq_t)primes[0];     /* primorial(1) = p1 */
        a_prev   = cache[1];
        cache_computed_up_to = 1;
    }

    /* return precomputed value if available */
    if ((size_t)n <= cache_computed_up_to)
        return cache[(size_t)n];

    /* extend cache up to n */
    for (size_t i = cache_computed_up_to + 1; i <= (size_t)n; ++i) {
        seq_t pi  = (seq_t)primes[i - 1];                /* p_i */
        seq_t new_a = pi * a_prev + Q_prev;             /* a(i) = p_i*a(i-1)+Q_{i-1} */
        cache[i] = new_a;

        /* update state for next iteration */
        Q_prev = Q_prev * pi;   /* primorial(i) */
        a_prev = new_a;
    }

    cache_computed_up_to = (size_t)n;
    return cache[(size_t)n];
}
