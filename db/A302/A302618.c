/*
 * A302618  Number of nX3 0..1 arrays with every element equal to 0, 1, 2 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * Empirical: a(n) = 2*a(n-1) + a(n-2) + 4*a(n-3) - 4*a(n-4) -
 *            4*a(n-5) * + a(n-6) - 4*a(n-7) - 4*a(n-9) (for n > 11) * Initial
 *            values (n ≥ 1): * a(1)=4, a(2)=17, a(3)=25, a(4)=71, a(5)=191,
 *            a(6)=498, * a(7)=1321, a(8)=3505, a(9)=9240, a(10)=24460,
 *            a(11)=64759
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

/* 0‑based unused entry is 0 so indices align with the recurrence */
static const seq_t init_seq[12] = {
    0,          /* dummy for 0 */
    4, 17, 25, 71, 191, 498, 1321, 3505, 9240, 24460, 64759 /* a(1) … a(11) */
};

seq_t a(idx_t n)
{
    if (n < 1u) return 0;   /* undefined for n < 1, return 0 */

    /* ---------- memoization structure ---------- */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size (in terms of indices) */
    static idx_t last  = 0;   /* highest index that has been computed */

    /* Allocate (or reallocate) enough room for indices 0 … n */
    if (cap <= n) {
        idx_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= n) new_cap <<= 1;        /* double until large enough */

        seq_t *new_mem = (seq_t*)malloc(sizeof(seq_t) * new_cap);
        if (!new_mem) {
            /* allocation failed – terminate the program (consistent with many libraries) */
            abort();
        }

        /* initialise the new array with a dummy value at index 0 */
        new_mem[0] = 0;

        /* copy whatever is already cached */
        for (idx_t i = 1; i <= last && i < new_cap; ++i) new_mem[i] = cache[i];

        /* free the old cache if it existed */
        if (cache) free(cache);

        cache = new_mem;
        cap   = new_cap;
    }

    /* ---------- initialise first 11 terms if needed ---------- */
    if (last < 11u) {
        idx_t from = last + 1;
        idx_t to   = (n < 11u) ? n : 11u;
        for (idx_t i = from; i <= to; ++i) cache[i] = init_seq[i];
        last = to;
    }

    /* ---------- compute remaining terms using the recurrence ---------- */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 11u) {               /* safety for very small n */
                cache[i] = init_seq[i];
            } else {
                seq_t val = 2u * cache[i-1]
                          + cache[i-2]
                          + 4u * cache[i-3]
                          - 4u * cache[i-4]
                          - 4u * cache[i-5]
                          + cache[i-6]
                          - 4u * cache[i-7]
                          - 4u * cache[i-9];
                cache[i] = val;
            }
        }
        last = n;
    }

    return cache[n];
}
