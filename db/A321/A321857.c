/*
 * A321857  a(n) = Pi(5,2)(n) + Pi(5,3)(n) - Pi(5,1)(n) - Pi(5,4)(n) where
 *          Pi(a,b)(x) denotes the number of primes in the arithmetic
 *          progression a*k + b less than or equal to x.
 * https://oeis.org/A321857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{prime p≤n} Legendre(p,5) * Equivalently, a(n) =
 *            (#non‑residues of p mod 5) – (#residues of p mod 5) * for primes
 *            p ≤ n (p ≠ 5). The implementation uses a sieve of * Eratosthenes
 *            to generate all primes ≤ n; a static cache holds the * list of
 *            primes and is extended only if a larger n is...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ----------  static cache of primes  ---------- */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;      /* number of primes stored */
static idx_t    cache_limit   = 0;      /* largest n cached so far */

/* Extend the prime cache to include all primes <= limit.
   The routine recomputes from scratch each time a larger limit
   is requested.  This is adequate for the small n range used
   in the sequence’s tests. */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= cache_limit) return;          /* already computed */

    /* free old list if it exists */
    if (prime_list) free(prime_list);
    prime_list = NULL;
    prime_count = 0;

    if (limit < 2) {                            /* no primes required */
        cache_limit = limit;
        return;
    }

    /* 0‑based array sized limit+1; allocate with cast for C++ */
    size_t sz = (size_t)(limit + 1);
    uint8_t *sieve = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!sieve) exit(1);                       /* allocation failure */
    sieve[0] = 1; sieve[1] = 1;                /* 0 and 1 are not prime */

    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!sieve[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = 1;
        }
    }

    /* Count primes first to allocate the array */
    size_t count = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!sieve[i]) ++count;

    prime_list = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!prime_list) exit(1);                   /* allocation failure */

    size_t k = 0;
    for (size_t i = 2; i < sz; ++i)
        if (!sieve[i]) prime_list[k++] = (uint64_t)i;

    prime_count = count;
    cache_limit = limit;

    free(sieve);
}

/* ----------  OEIS sequence function  ---------- */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined index safety */

    /* Prepare all primes up to n */
    ensure_primes_upto(n);

    seq_t residues   = 0;                      /* count of p ≡ 1,4 (mod 5) */
    seq_t nonresidues = 0;                     /* count of p ≡ 2,3 (mod 5) */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)n) break;            /* all remaining are too large */

        if (p == 5) continue;                  /* Legendre symbol is 0 */

        uint64_t r = p % 5;
        /* 1 and 4 are quadratic residues; 2 and 3 are non‑residues */
        if (r == 1 || r == 4)  residues++;
        else                   nonresidues++;
    }

    return nonresidues - residues;             /* difference as seq_t */
}
