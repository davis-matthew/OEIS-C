/*
 * A326568  Denominator of the average of the multiset of prime indices of n.
 * https://oeis.org/A326568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = k / gcd(S, k) where the multiset of prime indices of n is
 *            Σ e copies of the index of each prime factor p (with
 *            multiplicity e); S = Σ (index_of(p) * e), k = Σ e. The returned
 *            value is the denominator of the reduced average of this multiset
 * Range:     u8 n <= 7627, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

/* Cached data for prime indices */
static bool initialized = false;
static uint64_t *primes = NULL;      /* prime list (1‑based) */
static size_t prime_cnt = 0;         /* number of primes */
static idx_t *prime_index = NULL;    /* prime_index[p] = 1‑based index of prime p */
static const idx_t PRIME_LIMIT = 20000;   /* sufficient for n ≤ 10000 */

/* Initialise primes and prime_index table on first use. */
static void init_primes(void)
{
    if (initialized) return;

    /* Build the list of primes up to PRIME_LIMIT. */
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);

    /* Allocate the index array. */
    prime_index = (idx_t *)malloc((PRIME_LIMIT + 1) * sizeof(idx_t));
    if (!prime_index) exit(EXIT_FAILURE);  /* abort on allocation failure */

    /* Initially all entries are 0 (non‑prime). */
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i) prime_index[i] = 0;

    /* Fill index for each prime number. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        prime_index[primes[i]] = (idx_t)(i + 1);  /* 1‑based index */
    }

    initialized = true;
}

/* Main function computing the sequence term a(n). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for the offset; caller may ignore */

    init_primes();                /* ensure primes are ready */

    idx_t temp = n;
    seq_t S = 0;                  /* sum of (index * multiplicity) */
    idx_t k = 0;                  /* total multiplicity count */

    /* Trial division using precomputed primes. */
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p != 0) continue;

        /* Count exponent of p. */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        seq_t mul = (seq_t)prime_index[p] * (seq_t)e;
        S += mul;
        k += e;
    }

    /* Residual prime factor greater than sqrt(n). */
    if (temp > 1) {
        idx_t p = temp;
        seq_t mul = (seq_t)prime_index[p];  /* exponent is 1 */
        S += mul;
        ++k;
    }

    /* Compute the reduced denominator. */
    seq_t denom = (seq_t)k / gcd(S, (seq_t)k);
    return denom;
}
