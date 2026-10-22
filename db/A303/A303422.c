/*
 * A303422  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 4*a(n-2) + 21*a(n-3) + 18*a(n-4) (n>4) with
 *            a(1)=4, a(2)=32, a(3)=252, a(4)=1985
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: the sequence is defined for n ≥ 1 */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* cache[i] stores a(i+1)  (0‑based) */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of terms currently stored */

    /* Ensure capacity for at least n terms */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    while (len < (size_t)n) {
        idx_t i = len + 1;        /* 1‑based index of the term being computed */
        seq_t val;
        if (i == 1)      val = (seq_t)4;
        else if (i == 2) val = (seq_t)32;
        else if (i == 3) val = (seq_t)252;
        else if (i == 4) val = (seq_t)1985;
        else
            val = (seq_t)7  * cache[i - 2]   /* a(i-1) */
                + (seq_t)4  * cache[i - 3]   /* a(i-2) */
                + (seq_t)21 * cache[i - 4]   /* a(i-3) */
                + (seq_t)18 * cache[i - 5];  /* a(i-4) */

        cache[len] = val;
        ++len;
    }

    return cache[n - 1];
}
