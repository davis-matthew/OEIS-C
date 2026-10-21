/*
 * A276157  a(n) = {greatest primorial less than or equal to n} divided by
 *          {greatest primorial number which divides n} =
 *          A260188(n)/A053589(n).
 * https://oeis.org/A276157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A260188(n)/A053589(n)
 * Range:     u8 n <= 371, u16 n <= 9673, u32 n <= 9673, u64 n <= 9673,
 *            u128 n <= 9673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A260188(n)  – greatest primorial <= n               */
/* A053589(n)  – greatest primorial dividing n         */
/* (Primorial = product of consecutive primes starting from 2) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- prime table (up to 20000, more than enough for n≈10000) ---- */
#define MAX_PRIME_LIMIT 20000
#define MAX_PRIMES 2263   /* number of primes ≤ 20000 */

/* static storage for primes and a flag indicating initialization */
static size_t _prime_cnt = 0;
static uint64_t _primes[MAX_PRIMES];
static bool _primes_initialized = false;

/* initialize the prime list once */
static void _init_primes(void)
{
    if (_primes_initialized) return;
    _primes_initialized = true;

    bool sieve[MAX_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= MAX_PRIME_LIMIT; ++p) {
        if (sieve[p]) {
            for (size_t k = p * p; k <= MAX_PRIME_LIMIT; k += p)
                sieve[k] = false;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            _primes[_prime_cnt++] = i;
        }
    }
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* lazy initialisation of primes */
    _init_primes();

    if (n < 2) return 1;        /* a(1) = 1 is handled here */

    seq_t n_seq = (seq_t)n;     /* make sure we work in seq_t */
    seq_t pmax = 1;
    seq_t dmax = 1;

    /* greatest primorial <= n */
    for (size_t i = 0; i < _prime_cnt; ++i) {
        seq_t prime = (seq_t)_primes[i];
        if (pmax <= n_seq / prime) {
            pmax *= prime;
        } else {
            break;
        }
    }

    /* greatest primorial that divides n */
    for (size_t i = 0; i < _prime_cnt; ++i) {
        seq_t prime = (seq_t)_primes[i];
        if (prime > n_seq) break;          /* cannot divide any further */
        if (n_seq % prime == 0) {
            dmax *= prime;
        } else {
            break;
        }
    }

    return pmax / dmax;
}
