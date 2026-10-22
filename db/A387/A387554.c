/*
 * A387554  a(n) = (1/2) * Sum_{k=0..floor(n/2)} 2^(n-2*k) *
 *          binomial(2*k+2,2*n-4*k+1).
 * https://oeis.org/A387554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A387554
   a(n) = (1/2) * Σ_{k=0..⌊n/2⌋} 2^(n-2k) * C(2k+2, 2n-4k+1)
   Recurrence (valid for n ≥ 6):
     a(n) = 2*a(n-2) + 4*a(n-3) – a(n‑4) + 4*a(n‑5) – 4*a(n‑6)
   Initial values (n = 0 … 5):
     a(0)=1, a(1)=0, a(2)=2, a(3)=4, a(4)=3, a(5)=20
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = (idx_t)-1;

    /* First call – allocate and seed the cache with the first 6 terms. */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)4;   /* a(3) */
        cache[4] = (seq_t)3;   /* a(4) */
        cache[5] = (seq_t)20;  /* a(5) */
        last = 5;
    }

    /* Return a known value if possible. */
    if (n <= last)
        return cache[(size_t)n];

    /* Grow the cache so that it can store the requested index. */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap == 0) new_cap = 8;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failed – abort gracefully */
            return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute the sequence values in order, filling the cache. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Positive contributions  */
        seq_t p1 = (seq_t)2 * cache[(size_t)(i - 2)];
        seq_t p2 = (seq_t)4 * cache[(size_t)(i - 3)];
        seq_t p3 = (seq_t)4 * cache[(size_t)(i - 5)];
        seq_t pos = p1 + p2 + p3;

        /* Negative contributions */
        seq_t n1 = cache[(size_t)(i - 4)];
        seq_t n2 = (seq_t)4 * cache[(size_t)(i - 6)];
        seq_t neg = n1 + n2;

        /* The recurrence guarantees pos ≥ neg, so unsigned subtraction is safe. */
        cache[(size_t)i] = pos - neg;
    }

    last = n;
    return cache[(size_t)n];
}
