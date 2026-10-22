/*
 * A317420  a(n) = number of k with 1 <= k <= n-1 such that a(k) AND a(n-k) =
 *          a(k) (where AND denotes the bitwise AND operator).
 * https://oeis.org/A317420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2678, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Relation: a(n) = sum_{k=1}^{n-1} [ (a(k) & a(n-k)) == a(k) ]   */
/* (Where [P] denotes the Iverson bracket, 1 if P is true, 0 otherwise.) */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *CACHE = NULL;        /* storage for computed a(k) values */
static size_t CAP = 0;             /* allocated capacity of CACHE */
static idx_t  COMPUTED = 0;        /* highest index that has been filled */

static void ensure_capacity(idx_t n)
{
    /* Ensure CACHE can hold indices up to n (inclusive). */
    if ((size_t)n + 1 <= CAP)
        return;

    size_t new_cap = CAP ? CAP * 2 : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (CACHE) {
#ifdef __cplusplus
        for (size_t i = 0; i <= (size_t)COMPUTED; ++i)
            new_cache[i] = CACHE[i];
        delete [] CACHE;
#else
        memcpy(new_cache, CACHE, (COMPUTED + 1) * sizeof(seq_t));
        free(CACHE);
#endif
    }

    CACHE = new_cache;
    CAP = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n=0, but safe */

    ensure_capacity(n);

    /* Compute any missing values up to n */
    while (COMPUTED < n) {
        ++COMPUTED;                  /* next index to compute */

        if (COMPUTED == 1) {
            CACHE[1] = (seq_t)0;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < COMPUTED; ++k) {
            if ( (CACHE[k] & CACHE[COMPUTED - k]) == CACHE[k] )
                ++sum;
        }
        CACHE[COMPUTED] = sum;
    }

    return CACHE[n];
}
