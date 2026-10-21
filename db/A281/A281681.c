/*
 * A281681  a(n) = A055396(A071904(n)) - 1.
 * https://oeis.org/A281681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055396(A071904(n)) - 1 where A071904(n) = the n‑th odd
 *            composite number and A055396(m) = the 1‑based index of the
 *            smallest prime factor of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all primes up to a fixed limit using a
   Sieve of Eratosthenes.  For a given n, it scans odd integers for the
   n‑th odd composite, finds its smallest prime factor, looks up that
   prime's index, and returns (index-1).  The sieve limit is chosen to be
   large enough for n up to ~100 000 (much larger than the required 10 000)
   and thus guarantees correct results for the intended usage. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static constants for the sieve */
static const idx_t SIEVE_LIMIT = 500000;          /* 5×10⁵ suffices for n≈10⁴ */
static bool* is_prime = NULL;                    /* sieve flag array */
static uint64_t* primes = NULL;                 /* list of primes in ascending order */
static size_t prime_count = 0;                   /* number of primes found */
static int* prime_index = NULL;                  /* prime value → 1‑based index */

/* initialise the sieve, primes list and index array (once) */
static void init_sieve(void)
{
    if (is_prime != NULL)
        return;                                 /* already initialised */

    /* --- build the sieve flag array --- */
    is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);                     /* allocation failure */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* --- collect primes into an array --- */
    /* first count */
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            ++prime_count;

    primes = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!primes) exit(1);
    size_t ppos = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            primes[ppos++] = (uint64_t)i;

    /* --- build a quick lookup for prime position --- */
    prime_index = (int*)calloc((SIEVE_LIMIT + 1), sizeof(int));
    if (!prime_index) exit(1);
    for (size_t i = 0; i < prime_count; ++i)
        prime_index[primes[i]] = (int)(i + 1);   /* 1‑based index */
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                        /* undefined for n < 1 */

    init_sieve();

    /* find the n-th odd composite */
    idx_t count = 0;
    idx_t candidate = 9;                         /* first odd composite */
    while (1) {
        if (candidate > SIEVE_LIMIT) {
            /* For the required n this should never happen; fallback */
            return 0;
        }
        if (!is_prime[candidate]) {              /* composite */
            ++count;
            if (count == n)
                break;
        }
        candidate += 2;                          /* only odd numbers */
    }

    /* find smallest prime factor of candidate */
    uint64_t spf = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;                   /* skip even prime */
        if ((uint64_t)p * (uint64_t)p > candidate)
            break;
        if (candidate % p == 0) {
            spf = p;
            break;
        }
    }
    if (spf == 0)                            /* the candidate itself is prime? */
        spf = candidate;                     /* should not happen for composites */

    /* lookup prime position and return (index-1) */
    int pos = prime_index[(idx_t)spf];       /* 1‑based index; 0 if out of range */
    if (pos == 0) return 0;                  /* safety guard */
    return (seq_t)(pos - 1);
}
