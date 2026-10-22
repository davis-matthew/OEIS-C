/*
 * A357127  a(n) = A081257(n) if A081257(n) > n, otherwise a(n) = 1.
 * https://oeis.org/A357127
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A081257(n) if A081257(n) > n, else 1. * A081257(n) =
 *            greatest prime factor of n^2 + n + 1
 * Range:     u8 n <= 16, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* maximum prime needed for factorisation up to n ≈ 10000
   (sqrt(10000^2+10000+1) = 10001).  A generous margin is used. */
#define MAX_PRIME 200000U

/* prime sieve data */
static bool          sieve[MAX_PRIME + 1];
static uint32_t      primes[MAX_PRIME + 1];
static size_t        prime_count = 0;
static bool          primes_ready = false;

/* initialise the prime sieve once */
static void init_primes(void)
{
    if (primes_ready) return;
    for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!sieve[(size_t)i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                sieve[(size_t)j] = true;
        }
    }
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (!sieve[(size_t)i])
            primes[prime_count++] = (uint32_t)i;
    primes_ready = true;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;               /* behaviour below offset is irrelevant */

    init_primes();

    /* value whose greatest prime factor we need */
    idx_t val = (idx_t)n * (idx_t)n + (idx_t)n + 1;
    idx_t tmp = val;
    idx_t maxf = 1;

    /* trial division by pre‑computed primes */
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            maxf = p;                        /* track candidate largest prime factor */
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)                /* remaining prime factor */
        if (tmp > maxf) maxf = tmp;

    /* return it if it is greater than n, otherwise 1 */
    if (maxf > n) return (seq_t)maxf;
    else           return (seq_t)1;
}
