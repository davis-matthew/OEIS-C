/*
 * A394741  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are primes, not necessarily distinct, and y <= z.
 * https://oeis.org/A394741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the triples (x,y,z) of primes with x² + y*z = n and y ≤ z
   by brute force using a small prime sieve up to a fixed limit
   (200 000 is more than enough for n up to about 10 000).  The
   algorithm runs in O(n·π(√n)) time which easily meets the 2‑second
   requirement for n ≤ 10 000.  No external libraries are used besides
   the standard C headers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*   Fixed parameters and global storage                                 */
/* --------------------------------------------------------------------- */
#define MAX_PRIME_LIMIT 200000   /* upper bound for primes used */

static unsigned char _composite[MAX_PRIME_LIMIT + 1] = {0};
static uint32_t _primes[20000] = {0};
static size_t _prime_cnt = 0;
static bool _sieve_done = false;

/* --------------------------------------------------------------------- */
/*   Integer square‑root (floor)                                         */
/* --------------------------------------------------------------------- */
static idx_t int_sqrt(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        ++r;
    return r;
}

/* --------------------------------------------------------------------- */
/*   Prime sieve (once)                                                  */
/* --------------------------------------------------------------------- */
static void init_prime_sieve(void)
{
    if (_sieve_done)
        return;

    /* Sieve of Eratosthenes */
    _composite[0] = _composite[1] = 1;
    for (idx_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (!_composite[i])
            for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                _composite[j] = 1;

    for (idx_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!_composite[i])
            _primes[_prime_cnt++] = (uint32_t)i;

    _sieve_done = true;
}

/* --------------------------------------------------------------------- */
/*   Simple primality test using the precomputed sieve                  */
/* --------------------------------------------------------------------- */
static bool is_prime(idx_t p)
{
    if (p <= MAX_PRIME_LIMIT)
        return !_composite[(size_t)p];

    /* For safety, use trial division by primes up to sqrt(p) */
    idx_t r = int_sqrt(p);
    for (size_t i = 0; i < _prime_cnt && _primes[i] <= r; ++i)
        if (p % _primes[i] == 0)
            return false;
    return true;
}

/* --------------------------------------------------------------------- */
/*   Main sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_prime_sieve();

    seq_t count = 0;

    /* x must be prime and x² ≤ n */
    for (size_t ix = 0; ix < _prime_cnt &&
                        (idx_t)_primes[ix] * (idx_t)_primes[ix] <= n; ++ix)
    {
        idx_t x = _primes[ix];
        idx_t m = n - x * x;           /* m = y*z  */
        if (m < 2)
            continue;                  /* cannot be product of two primes */

        /* iterate over y ≤ √m */
        idx_t sqrt_m = int_sqrt(m);
        for (size_t iy = 0; iy < _prime_cnt &&
                            _primes[iy] <= sqrt_m; ++iy)
        {
            idx_t y = _primes[iy];
            if (m % y != 0)
                continue;

            idx_t z = m / y;
            if (z < y)                   /* enforce y ≤ z */
                continue;
            if (is_prime(z))
                ++count;
        }
    }

    return count;
}
