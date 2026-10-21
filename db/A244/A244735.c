/*
 * A244735  a(n) = (prime(n) mod 5) mod 2.
 * https://oeis.org/A244735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) mod 5) mod 2
 * Range:     u8 n <= 54, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A244735.  Memory and prime generation
   are handled lazily; results are cached for future calls. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static seq_t *_primes = NULL;     /* array of the primes discovered */
static idx_t  _prime_cnt = 0;     /* number of primes stored */
static idx_t  _prime_cap = 0;     /* allocated capacity of _primes */
static idx_t  _max_n_calculated = 0; /* largest n for which primes are known */

/* Estimate an upper bound for the n-th prime (Rosser's theorem). */
static inline idx_t upper_bound_for_n(idx_t n)
{
    if (n < 6) return 15;               /* first 5 primes are < 15    */
    double dn   = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));  /* Rosser's bound */
    return (idx_t)(bound) + 3;          /* safety margin             */
}

/* Append a prime to the dynamic array.  Capacity grows geometrically. */
static inline void add_prime(uint64_t p)
{
    if (_prime_cnt >= _prime_cap) {
        _prime_cap = _prime_cap ? _prime_cap * 2 : 32;
        _primes = (seq_t *)realloc((void*)_primes, _prime_cap * sizeof(seq_t));
    }
    _primes[_prime_cnt++] = (seq_t)p;
}

/* Simple sieve of Eratosthenes: fill _primes with all primes ≤ limit. */
static void sieve_up_to(idx_t limit)
{
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_prime) return;              /* allocation failure – leave empty */

    is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Reset the prime list and repopulate it. */
    _prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            add_prime(i);

    free(is_prime);
}

/* Ensure that the array _primes contains at least the first n primes. */
static void ensure_nth_prime(idx_t n)
{
    if (n == 0) return;
    if (n <= _max_n_calculated) return;          /* already available */

    idx_t new_limit = upper_bound_for_n(n);
    sieve_up_to(new_limit);

    _max_n_calculated = n;
}

/* Public function required by the task. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined below offset */
    ensure_nth_prime(n);

    seq_t p = _primes[n - 1];           /* n-th prime (1‑indexed)   */
    return (p % 5) % 2;                 /* (prime mod 5) mod 2      */
}
