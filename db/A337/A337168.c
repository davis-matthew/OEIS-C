/*
 * A337168  a(n) = (-1)^n + 2 * Sum_{k=0..n-1} a(k) * a(n-k-1).
 * https://oeis.org/A337168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + 2 * Sum_{k=0..n-1} a(k) * a(n-k-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values; persists across calls. */
    static seq_t *a_arr = NULL;      /* a values */
    static seq_t *conv  = NULL;      /* conv[i] = Σ_{k=0..i} a[k]*a[i-k] */
    static idx_t cap   = 0;          /* current allocated capacity */
    static idx_t last  = 0;          /* largest index already computed */

    /* Initialise on first call. */
    if (a_arr == NULL) {
        cap = 1;                     /* space for index 0 */
        a_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        conv  = (seq_t *)malloc(cap * sizeof(seq_t));
        a_arr[0] = (seq_t)1;         /* a(0) = 1 */
        conv[0]  = a_arr[0] * a_arr[0];   /* conv[0] = a0*a0 = 1 */
        last = 0;
    }

    /* Grow storage if we need to compute higher indices. */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap < n + 1)
            new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        conv  = (seq_t *)realloc(conv,  new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* 2 * Σ_{k=0..i-1} a[k] * a[i-1-k]  */
        seq_t twice = conv[i-1] + conv[i-1];

        /* a(i) = (-1)^i + 2 * Σ_{k=0..i-1} a[k] * a[i-1-k] */
        if ((i & 1) == 0)            /* even index: (-1)^i = +1 */
            a_arr[i] = twice + (seq_t)1;
        else                         /* odd index: (-1)^i = -1 */
            a_arr[i] = twice - (seq_t)1;

        /* Compute convolution for index i: Σ_{k=0..i} a[k] * a[i-k] */
        seq_t s = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k)
            s += a_arr[k] * a_arr[i - k];
        conv[i] = s;
    }

    last = (last > n) ? last : n;
    return a_arr[n];
}
