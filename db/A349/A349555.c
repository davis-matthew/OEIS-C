/*
 * A349555  a(n) = Sum_{p<=n, p prime} p^floor(1/gcd(n/p)).
 * https://oeis.org/A349555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p ≤ n, p prime} (p if p ∤ n else 1)
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalent to a(n) = A001221(n) + A066911(n)  (OEIS A349555) */
/*   Offset: 1, that is a(1) = 0, a(2) = 1, ...               */
/*   Computes values for all n ≥ 1 in any call order.           */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Prime cache: sieve of Eratosthenes up to the current maximum      */
/* ------------------------------------------------------------------ */
static idx_t  _sieve_limit   = 0;      /* highest n for which sieve was built */
static bool  *_sieve         = NULL;   /* boolean flags: true = composite   */
static uint64_t *_prime_list = NULL;   /* array of prime numbers           */
static size_t   _prime_count = 0;      /* number of primes in _prime_list  */

/* ------------------------------------------------------------------ */
/*  Ensure that all primes ≤ limit are in _prime_list with a sieve.  */
/* ------------------------------------------------------------------ */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _sieve_limit)
        return;                       /* already satisfied */

    /* Reallocate a fresh sieve array of size limit+1. */
    bool *new_sieve = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!new_sieve)                     /* allocation failed */
        return;

    if (_sieve)
        free(_sieve);
    _sieve = new_sieve;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                _sieve[j] = true;       /* mark as composite */
        }
    }

    /* Collect primes into a dynamic array */
    free(_prime_list);
    _prime_list = NULL;
    _prime_count = 0;

    size_t capacity = limit / 10 + 3;
    _prime_list = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    if (!_prime_list) {
        _prime_count = 0;
        _sieve_limit = limit;          /* still record limit */
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!_sieve[i]) {
            if (_prime_count >= capacity) {
                capacity *= 2;
                uint64_t *tmp = (uint64_t*)realloc(_prime_list,
                                                   capacity * sizeof(uint64_t));
                if (!tmp) {
                    free(_prime_list);
                    _prime_list = NULL;
                    _prime_count = 0;
                    _sieve_limit = limit;
                    return;
                }
                _prime_list = tmp;
            }
            _prime_list[_prime_count++] = (uint64_t)i;
        }
    }

    _sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)          /* for n=1, no primes ≤ n */
        return 0;

    _ensure_primes_upto(n);

    seq_t sum = 0;
    for (size_t i = 0; i < _prime_count; ++i) {
        uint64_t p = _prime_list[i];
        if (p > (uint64_t)n)       /* should not happen, but guard */
            break;

        if (n % p == 0)            /* prime divides n */
            sum += 1;
        else
            sum += (seq_t)p;       /* otherwise add the prime itself */
    }

    return sum;
}
