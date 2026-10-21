/*
 * A247681  Odd nonprimes congruent to 1 modulo 9.
 * https://oeis.org/A247681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th odd composite number congruent to 1 (mod 9);
 *            a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 2562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utils provide a generic prime test. */
#include "../utils/isPrimeGeneric.h"
#include <stdbool.h>
#include <stdlib.h>

/* Memoisation table for already computed terms. */
static seq_t *cache = NULL;      /* array of previous terms          */
static size_t  cap   = 0;        /* current allocated capacity       */
static size_t  len   = 0;        /* number of terms stored           */
static idx_t   next_m = 0;       /* next multiplier m to try         */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_capacity(size_t need) {
    if (cap < need) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap < need) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* abort on memory exhaustion */
        cache = tmp;
        cap   = newcap;
    }
}

/* Return the n‑th odd nonprime ≡ 1 (mod 9).  n >= 1. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;        /* no a(0) defined */

    /* Generate further terms until we have at least n of them. */
    while (len < (size_t)n) {
        /* Numbers of the form 9·m + 1.  m starts at 0 and grows. */
        seq_t candidate = (seq_t)9 * (seq_t)next_m + (seq_t)1;
        next_m++;                              /* next m for next round */

        /* Skip even candidates: we need only odd numbers. */
        if ((candidate & (seq_t)1) == 0) continue;

        /* Candidate is nonprime if it is 1 or the generic prime test
           says it is not prime. */
        bool nonprime = (candidate == (seq_t)1) || !isPrimeGeneric(candidate);
        if (nonprime) {
            ensure_capacity(len + 1);
            cache[len++] = candidate;
        }
    }
    return cache[n - 1];
}
