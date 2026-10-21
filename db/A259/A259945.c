/*
 * A259945  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00000101.
 * https://oeis.org/A259945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259945
   Empirical recurrence:
   a(1)=34, a(2)=60, a(3)=156, a(4)=365, a(5)=801, a(6)=1825,
   a(7)=4272, a(8)=9840, a(9)=22444
   for n > 9:
   a(n)=a(n‑1)+a(n‑2)+2*a(n‑3)+6*a(n‑4)–a(n‑6)+a(n‑7)–a(n‑8) */

#include <stdlib.h>
#include <stdint.h>

static const seq_t _base_values[10] = {
    0ULL,          /* dummy 0‑th index */
    (seq_t)34ULL,  /* a(1) */
    (seq_t)60ULL,  /* a(2) */
    (seq_t)156ULL, /* a(3) */
    (seq_t)365ULL, /* a(4) */
    (seq_t)801ULL, /* a(5) */
    (seq_t)1825ULL,/* a(6) */
    (seq_t)4272ULL,/* a(7) */
    (seq_t)9840ULL,/* a(8) */
    (seq_t)22444ULL/* a(9) */
};

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1 – undefined for 0 */
        return 0;

    /* Static memoisation cache, 1‑based indexing */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;     /* current allocated size (terms) */
    static idx_t  last_computed = 0;  /* largest index computed so far */

    /* Ensure cache large enough for index n */
    if (n + 1 > cache_size) {
        size_t new_capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0, unlikely */
            return 0;
        cache       = tmp;
        cache_size  = new_capacity;
    }

    /* Compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 9)
            cache[i] = _base_values[i];
        else
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)2 * cache[i-3]
                       + (seq_t)6 * cache[i-4] - cache[i-6]
                       + cache[i-7] - cache[i-8];
    }
    last_computed = n;
    return cache[n];
}
