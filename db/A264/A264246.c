/*
 * A264246  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 1,1 0,-1 -1,1 or
 *          0,-2.
 * https://oeis.org/A264246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1) + 8*a(n-2) + 24*a(n-3) + 88*a(n-4) + 88*a(n-5) + 24*a(n-6)
          - 552*a(n-7) - 352*a(n-8) - 96*a(n-9)    for n > 10
   Initial values: a(1)=3, a(2)=21, a(3)=95, a(4)=557, a(5)=3303, a(6)=18725,
                  a(7)=105839, a(8)=602365, a(9)=3432551, a(10)=19548549. */

#include <stdlib.h>

static const seq_t init[10] = {
    3, 21, 95, 557, 3303, 18725, 105839, 602365, 3432551, 19548549
};

seq_t a(idx_t n)
{
    /* Empty or out‑of‑range requests */
    if (n == 0) return (seq_t)0;

    /* Static cache: arr[0] = a(1), arr[1] = a(2), etc. */
    static seq_t *arr = NULL;
    static size_t len = 0;   /* number of terms that have been computed */
    static size_t cap = 0;   /* allocated capacity in elements */

    /* First call: allocate space for at least the initial terms. */
    if (arr == NULL) {
        cap = (n > 10 ? n : 10);          /* de‑finitely enough for init */
        arr = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!arr) return (seq_t)0;        /* allocation failure -> 0 */
        for (size_t i = 0; i < 10; ++i)   /* copy initial values */
            arr[i] = init[i];
        len = 10;                         /* we have 10 terms ready */
    }

    /* If the requested index is already known, return it immediately. */
    if (n <= len)
        return arr[n - 1];

    /* Grow the cache if needed. */
    if (cap < (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t*)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure -> 0 */
        arr = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t i = (idx_t)len; i < n; ++i) { /* i is 0‑based for arr */
        /* i >= 10 because len >= 10 after initialization,
           so i-9 is always non‑negative. */
        seq_t v = 3 * arr[i - 1]                        /* a(i)   */
                + 8 * arr[i - 2]                        /* a(i-1) */
                + 24 * arr[i - 3]                       /* a(i-2) */
                + 88 * arr[i - 4]                       /* a(i-3) */
                + 88 * arr[i - 5]                       /* a(i-4) */
                + 24 * arr[i - 6]                       /* a(i-5) */
                - 552 * arr[i - 7]                      /* a(i-6) */
                - 352 * arr[i - 8]                      /* a(i-7) */
                - 96  * arr[i - 9];                     /* a(i-8) */
        arr[i] = v;
    }
    len = n;                                  /* we now have terms 1..n */

    return arr[n - 1];
}
