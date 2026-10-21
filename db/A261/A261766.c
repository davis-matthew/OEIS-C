/*
 * A261766  a(n) is the number of partial derangements of an n-set with at
 *          least one orbit of size exactly n.
 * https://oeis.org/A261766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! + n! for n>=2, a(0)=1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the definition:
   a(n) counts partial derangements of an n‑set that contain
   an orbit of size exactly n, which are exactly the n‑cycles
   and the n‑length chains. There are (n-1)! cycles and n! chains.
   For n=1 no such orbits exist, and a(0)=1 is given. */
#include <stdlib.h>

/* static cache of factorials up to the maximum n seen so far   */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!          */
static size_t fact_size = 0;       /* number of factorials stored   */

/* Ensure that factorials up to n are cached */
static void ensure_factorial_cache(idx_t n)
{
    if (fact_cache == NULL) {
        /* allocate initial cache for 0! */
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        fact_size = 1;
        fact_cache[0] = (seq_t)1;  /* 0! = 1 */
    }
    /* extend the cache if necessary */
    while (fact_size <= (size_t)n) {
        /* next factorial */
        fact_cache[fact_size] = fact_cache[fact_size - 1] * ((seq_t)fact_size);
        fact_size++;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    if (n == 1) return (seq_t)0;          /* a(1) = 0 */
    ensure_factorial_cache(n);            /* make sure n! and (n-1)! are ready */
    return fact_cache[n] + fact_cache[n - 1];
}
