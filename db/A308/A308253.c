/*
 * A308253  Number of integer-sided triangles with perimeter n and nonprime
 *          side lengths.
 * https://oeis.org/A308253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles of perimeter n * where
 *            all three side lengths are nonprime (including 1). The *
 *            computation does a direct double loop over all ordered triples *
 *            a ≤ b ≤ c with a + b + c = n, testing the three side lengths *
 *            for non‑primality using a dynamic sieve of Erato...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Dynamic prime table ------------------------------------------------ */

/* prime_tbl[i] == 1  →  i is prime
 * prime_tbl[i] == 0  →  i is composite or 0 or 1                                         */
static idx_t prime_cap = 0;          /* size of prime_tbl in elements          */
static uint8_t *prime_tbl = NULL;    /* pointer to the table                   */

/* Allocate at least (n+1) entries.  Existing entries are preserved
 * if they are still required.  All new entries are initialized to
 * "prime" (1) before the sieve marks composites. */
static void ensure_primes(idx_t n)
{
    if (n + 1 <= prime_cap)
        return;

    uint8_t *new_tbl = (uint8_t *)realloc(prime_tbl, (n + 1) * sizeof(uint8_t));
    if (!new_tbl)
        abort();
    prime_tbl = new_tbl;

    /* Initially assume all numbers >=2 are prime. */
    for (idx_t i = 0; i <= n; ++i)
        prime_tbl[i] = 1;
    if (n >= 0) prime_tbl[0] = 0;
    if (n >= 1) prime_tbl[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= n; ++p)
        if (prime_tbl[p])
            for (idx_t m = p * p; m <= n; m += p)
                prime_tbl[m] = 0;

    prime_cap = n + 1;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether a positive integer is non‑prime               */

static inline bool nonprime(idx_t x)
{
    if (x == 0) return true;      /* never used, but safe */
    if (x == 1) return true;      /* by definition */
    ensure_primes(x);
    return !prime_tbl[x];
}

/* ------------------------------------------------------------------ */
/* Sequence cache                                                      */

static seq_t *cache = NULL;       /* memoised results */
static uint8_t *cached = NULL;    /* 0 = uncomputed, 1 = computed */
static idx_t cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */

seq_t a(idx_t n)
{
    /* Expand caches if necessary */
    if (n + 1 > cache_cap) {
        seq_t *newc = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        uint8_t *newp = (uint8_t *)realloc(cached, (n + 1) * sizeof(uint8_t));
        if (!newc || !newp)
            abort();

        /* Zero any newly added entries */
        if (cache_cap > 0) {
            memset(newc + cache_cap, 0,
                   (n + 1 - cache_cap) * sizeof(seq_t));
            memset(newp + cache_cap, 0,
                   (n + 1 - cache_cap) * sizeof(uint8_t));
        }

        cache = newc;
        cached = newp;
        cache_cap = n + 1;
    }

    if (cached[n])
        return cache[n];

    /* Prepare prime table for side lengths up to n */
    ensure_primes(n);

    seq_t count = 0;
    for (idx_t a_side = 1; a_side <= n / 3; ++a_side) {
        for (idx_t b_side = a_side; b_side <= (n - a_side) / 2; ++b_side) {
            idx_t c_side = n - a_side - b_side;
            if (c_side < b_side)                /* maintain a ≤ b ≤ c */
                continue;
            if (a_side + b_side <= c_side)      /* triangle inequality */
                continue;

            /* All sides must be non‑prime */
            if (!nonprime(a_side) ||
                !nonprime(b_side) ||
                !nonprime(c_side))
                continue;

            ++count;
        }
    }

    cached[n] = 1;
    cache[n] = count;
    return count;
}
