/*
 * A275132  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275132: number of 3×n 0..2 arrays with no element equal to values at
   offsets (-2,-2), (-1,0) or (-1,1).
   Recurrence for n>6 (1‑based indexing):
      a(n) = 3*a(n-1) + 2*a(n-2) + 6*a(n-3) - 2*a(n-4) - 4*a(n-5)
   Initial values:
      a(1)=2, a(2)=12, a(3)=45, a(4)=174, a(5)=675, a(6)=2607. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                    /* behaviour below offset does not matter */
        return 0;

    static seq_t *cache = NULL;   /* memoised values */
    static idx_t cache_size = 0;  /* largest n cached so far */

    if (cache == NULL || n > cache_size) {
        /* enlarge cache to hold indices up to n (1‑based) */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();           /* allocation failed */
        cache = tmp;

        /* initialise any new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)          cache[i] = 2;
            else if (i == 2)     cache[i] = 12;
            else if (i == 3)     cache[i] = 45;
            else if (i == 4)     cache[i] = 174;
            else if (i == 5)     cache[i] = 675;
            else if (i == 6)     cache[i] = 2607;
            else {
                /* recurrence using only built‑in operators, safe for
                   small types up to the 64‑bit limit; for expanded
                   precision types the operators are overloaded. */
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                cache[i] = 3*a1 + 2*a2 + 6*a3 - 2*a4 - 4*a5;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
