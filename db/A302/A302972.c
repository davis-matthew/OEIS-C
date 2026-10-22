/*
 * A302972  a(n) is the smallest integer r such that A002828(r*n) = 4.
 * https://oeis.org/A302972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the smallest r such that the squarefree part of r*n is congruent to 7 modulo 8 */
#include <stdint.h>
#include <stdbool.h>

/* Prime sieve data */
#define PRIME_LIMIT 100000
#define MAX_PRIMES  10000  /* enough for PRIME_LIMIT */

static unsigned char _is_prime[PRIME_LIMIT + 1];
static unsigned int   _primes[MAX_PRIMES];
static unsigned int   _prime_count = 0;
static bool           _primes_inited = false;

/* Build the list of primes up to PRIME_LIMIT */
static void init_primes(void)
{
    if (_primes_inited) return;
    _primes_inited = true;

    /* Sieve of Eratosthenes */
    for (unsigned int i = 0; i <= PRIME_LIMIT; ++i) _is_prime[i] = 0xFF;
    _is_prime[0] = _is_prime[1] = 0;
    for (unsigned int i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (_is_prime[i])
        {
            for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                _is_prime[j] = 0;
        }
    }
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (_is_prime[i])
        {
            if (_prime_count < MAX_PRIMES)
                _primes[_prime_count++] = i;
        }
    }
}

/* Return the squarefree part of n modulo 8 (value 0..7) */
static unsigned char squarefree_mod8(idx_t n)
{
    unsigned char res = 1;          /* product modulo 8 */
    idx_t tmp = n;
    for (unsigned int i = 0; i < _prime_count; ++i)
    {
        unsigned int p = _primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p == 0)
        {
            unsigned int parity = 0;
            do
            {
                tmp /= p;
                parity ^= 1;
            } while (tmp % p == 0);
            if (parity)                  /* odd exponent, include p */
                res = (res * (p & 7)) & 7;   /* mod 8 */
        }
    }
    if (tmp > 1)                      /* remaining prime factor > sqrt(n) */
        res = (res * (tmp & 7)) & 7;
    return res;
}

/* Public function: a(n) = smallest r >= 1 with squarefree(r*n) ≡ 7 (mod 8) */
seq_t a(idx_t n)
{
    init_primes();
    idx_t r = 1;
    /* In practice r is small; loop until condition is satisfied */
    while (1)
    {
        idx_t prod = n * r;
        if (squarefree_mod8(prod) == 7) break;
        ++r;
    }
    return (seq_t)r;
}
