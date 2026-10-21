/*
 * A242124  a(n) = n-th prime modulo 26.
 * https://oeis.org/A242124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime modulo 26
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a growing cache of primes using the supplied
 * isPrimeTrial() routine.  For an arbitrary order of calls it always
 * generates at least n primes and returns the requested one mod 26. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

static uint64_t *primes   = NULL;     /* prime list (0‑based)            */
static size_t   primes_cnt = 0;       /* current number of stored primes */
static size_t   primes_cap = 0;       /* allocated capacity               */
static uint64_t candidate   = 2;      /* next integer to test for prime  */

/* Ensure the primes array has room for at least 'need' elements. */
static void grow_primes_cap(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t cap = primes_cap ? primes_cap : 16;
    while (cap < need)
        cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
    if (!tmp)
        abort(); /* out of memory */
    primes = tmp;
    primes_cap = cap;
}

/* Generate primes until we have at least 'needed' of them. */
static void ensure_primes_at_least(size_t needed)
{
    if (primes_cnt >= needed)
        return;
    grow_primes_cap(needed);
    while (primes_cnt < needed) {
        /* Find the next prime starting from 'candidate'. */
        while (!isPrimeTrial((seq_t)candidate))
            ++candidate;
        primes[primes_cnt++] = candidate;
        ++candidate; /* advance to next integer for subsequent searches */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n == 0 but return 0 for safety */

    /* Ensure we have at least n primes cached. */
    ensure_primes_at_least((size_t)n);

    /* Return the n-th prime modulo 26. */
    uint64_t p = primes[(size_t)(n - 1)];
    return (seq_t)(p % 26);
}
