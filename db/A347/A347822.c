/*
 * A347822  Number of ways to write n as the sum of 2 balanced numbers.
 * https://oeis.org/A347822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..floor(n/2)} c(k) * c(n-k) where c(k)=1 if k is
 *            a balanced number else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced number: φ(k) divides σ(k).  The implementation computes φ and σ for all numbers up to n
   using simple sieves and counts the number of pairs of balanced numbers that add to n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for computed values up to some limit */
static idx_t _bal_limit = 0;
static uint64_t *_phi = NULL;          /* φ values */
static uint64_t *_sigma = NULL;        /* σ values */
static unsigned char *_is_balanced = NULL; /* 1 if balanced, 0 otherwise */

/* Grow the cached arrays to at least new_limit (inclusive).  Properly
   cast the result of realloc for C++ compatibility. */
static void _recompute_up_to(idx_t new_limit)
{
    if (_bal_limit >= new_limit)
        return;

    /* Allocate or grow arrays */
    _phi = (uint64_t *)realloc(_phi, (new_limit + 1) * sizeof(uint64_t));
    _sigma = (uint64_t *)realloc(_sigma, (new_limit + 1) * sizeof(uint64_t));
    _is_balanced = (unsigned char *)realloc(_is_balanced,
                                            (new_limit + 1) * sizeof(unsigned char));
    if (!_phi || !_sigma || !_is_balanced)
        return; /* allocation failure – nothing further can be done */

    /* Initialise φ to n (on orders 0..new_limit) */
    for (idx_t i = 0; i <= new_limit; ++i)
        _phi[i] = (uint64_t)i;

    /* Sieve φ */
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (_phi[i] == (uint64_t)i)          /* i is prime */
        {
            for (idx_t j = i; j <= new_limit; j += i)
                _phi[j] -= _phi[j] / i;
        }
    }

    /* Initialise σ to 0 */
    for (idx_t i = 0; i <= new_limit; ++i)
        _sigma[i] = 0;

    /* Sieve σ by adding divisors */
    for (idx_t i = 1; i <= new_limit; ++i)
    {
        for (idx_t j = i; j <= new_limit; j += i)
            _sigma[j] += i;
    }

    /* Determine which numbers are balanced */
    for (idx_t i = 1; i <= new_limit; ++i)
        _is_balanced[i] = (_sigma[i] % _phi[i] == 0);

    _bal_limit = new_limit;
}

/* OEIS sequence A347822: number of ways to write n as the sum of two balanced numbers. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* Not defined for 0, but safe guard */

    _recompute_up_to(n);

    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k)
    {
        if (_is_balanced[k] && _is_balanced[n - k])
            count++;
    }

    return count;
}
