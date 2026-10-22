/*
 * A378372  Least non prime power >= n, allowing 1.
 * https://oeis.org/A378372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least non prime power >= n (1 is allowed) where a(n)
 *            = A378371(n) + n (A378371 gives the smallest non prime power ≥
 *            n)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve – static initialisation of a list of small primes.
 *  The limit chosen (100 000) is safe for all reasonable n (even up to 10⁶) 
 *  and keeps memory usage negligible.                                          */
/* ------------------------------------------------------------------ */

#define PRIMES_SIEVE_LIMIT 100000

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void) {
    if (primes_init)
        return;

    /* Simple sieve of Eratosthenes up to PRIMES_SIEVE_LIMIT. */
    bool *is_composite = (bool *)malloc((PRIMES_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_composite)
        return; /* allocation failed – leave primes=NULL and primes_cnt=0 */

    for (size_t i = 2; i <= PRIMES_SIEVE_LIMIT; ++i)
        is_composite[i] = false;

    for (size_t p = 2; p * p <= PRIMES_SIEVE_LIMIT; ++p) {
        if (!is_composite[p]) {
            for (size_t j = p * p; j <= PRIMES_SIEVE_LIMIT; j += p)
                is_composite[j] = true;
        }
    }

    /* Count primes and store them. */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIMES_SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        return; /* allocation failed – leave primes=NULL */
    }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIMES_SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            primes[idx++] = (uint64_t)i;

    primes_cnt = cnt;
    primes_init = true;
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*  Test whether a number x (>1) is a prime power.
 *  Only allows a single distinct prime factor.
 *  1 is explicitly not a prime power.                                 */
/* ------------------------------------------------------------------ */

static bool is_prime_power(seq_t x) {
    if (x <= (seq_t)1)
        return false;                /* 1 is not a prime power */

    seq_t temp = x;
    int distinct = 0;               /* number of distinct prime factors found */

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        /* stop when p^2 > temp to reduce work */
        if (p * p > temp)
            break;

        bool divided = false;
        while (temp % p == 0) {
            temp /= p;
            divided = true;
        }
        if (divided) {
            ++distinct;
            if (distinct > 1)
                return false;        /* more than one distinct prime factor */
        }
        if (temp == (seq_t)1)
            break;
    }

    if (temp > (seq_t)1)
        ++distinct;                   /* remaining factor is prime */

    return distinct == 1;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function
 *  a(n): least non prime power >= n (1 is allowed).                    */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    init_primes();                    /* ensure prime table is ready */

    seq_t m = (seq_t)n;
    while (is_prime_power(m))
        ++m;
    return m;
}
