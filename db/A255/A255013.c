/*
 * A255013  a(n) = the least semiprime such that n+a(n) is also semiprime.
 * https://oeis.org/A255013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least semiprime s such that n+s is also semiprime
   (OEIS A255013).  Works for any seq_t/idx_t pair supported by the
   build. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Prime sieve limits the maximum prime needed for factorisation.
   With n up to about 10000 the corresponding s and n+s are far below
   one million, so 200 000 is more than sufficient. */
static const idx_t PRIME_LIMIT = 200000U;
static uint64_t *g_primes = NULL;
static size_t g_prime_cnt = 0;

/* Initialise the global prime table once. */
static void init_primes(void)
{
    if (g_primes != NULL)
        return;                               /* already initialised */
    build_primes_upto(PRIME_LIMIT, &g_primes, &g_prime_cnt);
}

/* Return true iff x is a semiprime: product of exactly two primes. */
static bool is_semiprime(seq_t x)
{
    if (x < (seq_t)4)            /* 2*2 is the smallest semiprime */
        return false;

    init_primes();              /* prime list ready */

    seq_t tmp   = x;            /* value to be factorised */
    int  fcnt   = 0;            /* number of prime factors found */

    /* Pre‑compute floor(sqrt(tmp)).  For seq_t that is small the
       square root will also fit comfortably in a uint64_t, so a
       cast is safe. */
    seq_t sqrt_seq = floor_sqrt(tmp);
    uint64_t sqrt_val = (uint64_t)sqrt_seq;

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p64 = g_primes[i];
        if (p64 < 2)               /* safety for tiny seq_t types */
            continue;
        if (p64 > sqrt_val)        /* no further useful divisors */
            break;

        seq_t p = (seq_t)p64;      /* safe: p <= sqrt_val <= seq_t max */
        while (tmp % p == 0) {
            ++fcnt;
            tmp /= p;
            if (fcnt > 2)          /* not a semiprime any more */
                return false;
        }
    }

    /* If a residue > 1 remains it contributes a prime factor. */
    if (tmp > (seq_t)1)
        ++fcnt;

    return fcnt == 2;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 0 */

    seq_t nseq = (seq_t)n;
    seq_t s    = (seq_t)4;          /* first possible semiprime */

    for (;;) {
        if (is_semiprime(s) && is_semiprime(nseq + s))
            return s;
        ++s;
    }
}
