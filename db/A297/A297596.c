/*
 * A297596  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A297596
   Empirical recurrence:
     a(n) = a(n-1) + a(n-2) + 8*a(n-3) + 7*a(n-4) - 8*a(n-6) - 6*a(n-7)
     with a(1)=1, a(2)=9, a(3)=25, a(4)=49, a(5)=145, a(6)=443, a(7)=1141
*/

#include <stdlib.h>

static seq_t *cache = NULL;           /* array holding computed values */
static idx_t  cache_cap = 0;          /* total allocated slots (0..cap-1) */
static idx_t  max_computed = 0;       /* largest index with a value stored */

/* Ensure that the array has room for indices 0 .. n inclusive.
   Reallocates if needed and casts to seq_t* (must be done for C++). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1) * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1); /* overflow guard */

        /* Use explicit cast to satisfy C++ requirements */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failed – caller will see zero */

        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }
}

/* Compute the nth term of sequence A297596.  n>=1 is guaranteed. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset not used – safe return */

    /* Initialize cache up to index 7 with known values. */
    if (max_computed < 7) {
        ensure_capacity(7);
        cache[1] =  (seq_t)1;
        cache[2] =  (seq_t)9;
        cache[3] =  (seq_t)25;
        cache[4] =  (seq_t)49;
        cache[5] =  (seq_t)145;
        cache[6] =  (seq_t)443;
        cache[7] =  (seq_t)1141;
        max_computed = 7;
    }

    if (n <= max_computed) return cache[(size_t)n];

    /* Compute successive terms up to n using the empirical recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += cache[i - 2];
        val += (seq_t)8 * cache[i - 3];
        val += (seq_t)7 * cache[i - 4];
        val -= (seq_t)8 * cache[i - 6];
        val -= (seq_t)6 * cache[i - 7];

        ensure_capacity(i);
        cache[i] = val;
    }
    max_computed = n;
    return cache[(size_t)n];
}
