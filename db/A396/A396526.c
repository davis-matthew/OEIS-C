/*
 * A396526  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 71^2)^2 = y^2.
 * https://oeis.org/A396526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396526
   a(n) = 6*a(n-5) - a(n-10) + 10082  for n >= 11
   a(1) = 0,  a(2) = 923,  a(3) = 2040,  a(4) = 8379,
   a(5) = 11360,  a(6) = 15123,  a(7) = 19880,
   a(8) = 25899,  a(9) = 61820,  a(10) = 79023
   (terms below n = 1 are undefined). */

#include <stdlib.h>   /* malloc, free, exit */

#ifdef __cplusplus
/* Compiler is C++: use operator new/delete */
static seq_t *alloc_cache(size_t n) {
    return new seq_t[n];
}
static void free_cache(seq_t *p) {
    delete[] p;
}
#else
/* Pure C compiler */
static seq_t *alloc_cache(size_t n) {
    return (seq_t *)malloc(n * sizeof(seq_t));
}
static void free_cache(seq_t *p) {
    free(p);
}
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* index 0 unused */
    static idx_t cache_len = 0;      /* highest computed index */

    if (n < 1) return (seq_t)0;      /* out‑of‑range is never used */

    /* Expand cache if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)(n + 1);   /* allow indices 0..n */
        seq_t *new_cache = alloc_cache(new_len);

        /* Copy already computed values */
        for (idx_t i = 1; i <= cache_len; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 10) {   /* base cases */
                switch (i) {
                    case 1: new_cache[1]  = (seq_t)0;     break;
                    case 2: new_cache[2]  = (seq_t)923;   break;
                    case 3: new_cache[3]  = (seq_t)2040;  break;
                    case 4: new_cache[4]  = (seq_t)8379;  break;
                    case 5: new_cache[5]  = (seq_t)11360; break;
                    case 6: new_cache[6]  = (seq_t)15123; break;
                    case 7: new_cache[7]  = (seq_t)19880; break;
                    case 8: new_cache[8]  = (seq_t)25899; break;
                    case 9: new_cache[9]  = (seq_t)61820; break;
                    case 10: new_cache[10] = (seq_t)79023; break;
                }
            } else {   /* recurrence for n >= 11 */
                new_cache[i] = (seq_t)6 * new_cache[i-5]
                               - new_cache[i-10]
                               + (seq_t)10082;
            }
        }

        /* Replace old cache */
        if (cache) free_cache(cache);
        cache = new_cache;
        cache_len = n;
    }

    return cache[n];
}
