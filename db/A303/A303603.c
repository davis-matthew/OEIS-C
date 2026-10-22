/*
 * A303603  a(n) is the maximum distance between primes in Goldbach partitions
 *          of 2n, or 2n if there are no Goldbach partitions of 2n.
 * https://oeis.org/A303603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{p+q=2n, p,q prime} (q-p), or 2n if no such Goldbach
 *            pair exists. The first value a(1)=0
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- prime sieve helpers --------------------------------------------- */
static idx_t sieve_limit = 0;          /* largest value that has been sieved */
static unsigned char *sieve_arr = NULL;/* 1 = prime, 0 = composite */

/* Allocate a new sieve array for numbers up to 'limit' (inclusive). */
static void
build_sieve(idx_t limit)
{
    /* Release old sieve if it exists. */
    if (sieve_arr) {
        free(sieve_arr);
        sieve_arr = NULL;
    }

    /* Allocate raw memory and initialise all entries to 1 (prime). */
    sieve_arr = (unsigned char *)malloc((size_t)(limit + 1));
    if (!sieve_arr) {
        /* Allocation failure – leave array NULL (caller will treat as no sieve). */
        return;
    }
    for (idx_t i = 0; i <= limit; ++i)
        sieve_arr[i] = 1;

    /* 0 and 1 are not prime. */
    if (limit >= 0) sieve_arr[0] = 0;
    if (limit >= 1) sieve_arr[1] = 0;

    /* Eratosthenes sieve. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!sieve_arr[p]) continue;          /* composite */
        for (idx_t q = p * p; q <= limit; q += p)
            sieve_arr[q] = 0;                  /* mark multiples composite */
    }

    sieve_limit = limit;
}

/* Ensure we have a sieve covering at least 'limit'. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;         /* already sufficient */
    build_sieve(limit);
}

/* --------------------------------------------------------------------- */

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is defined as 0 for coherence. */
    if (n == 1) return (seq_t)0;

    /* we will examine pairs (p, q) with p+q = 2n, p <= q. */
    const idx_t total = 2 * n;                 /* even number to split */
    ensure_sieve(total);                       /* prime table up to 2n */

    if (!sieve_arr)                            /* sieve failed – fall back */
        return (seq_t)total;

    /* Search for the smallest prime p such that q = total-p is also prime.
       That yields the maximal distance. */
    for (idx_t p = 2; p <= n; ++p) {           /* p cannot exceed n */
        if (!sieve_arr[p]) continue;           /* p not prime */
        const idx_t q = total - p;
        if (q < p) break;                      /* avoid duplicates */
        if (!sieve_arr[q]) continue;           /* q not prime */
        /* Found the Goldbach pair with minimal p ⇒ maximal q-p. */
        return (seq_t)(q - p);
    }

    /* No Goldbach partition found – return the value 2n itself. */
    return (seq_t)total;
}
