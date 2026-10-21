/*
 * A296329  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A296329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A296329: Number of n×2 0..1 arrays with each 1 horizontally,
   vertically or antidiagonally adjacent to 0 or 2 neighboring 1s.
   Recurrence (for n ≥ 6):
   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4) + a(n-5)
   Initial values:
   a(1)=3, a(2)=8, a(3)=19, a(4)=47, a(5)=116
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence starts at index 1. */
    if (n == 0) return 0;

    /* Static cache: arr[0] holds a(1), arr[1] holds a(2), … */
    static seq_t *arr = NULL;
    static idx_t sz   = 0;   /* number of terms currently stored */

    /* First call: initialise the cache with the five given terms. */
    if (!arr) {
        sz = 5;
        arr = (seq_t *)malloc(sizeof(seq_t) * sz);
        if (!arr) return 0; /* allocation failure; give up */

        arr[0] = (seq_t)3;   /* a(1) */
        arr[1] = (seq_t)8;   /* a(2) */
        arr[2] = (seq_t)19;  /* a(3) */
        arr[3] = (seq_t)47;  /* a(4) */
        arr[4] = (seq_t)116; /* a(5) */
    }

    /* If the requested term is already computed, return it. */
    if (n <= sz) return arr[n - 1];

    /* Ensure the underlying array has room for n terms. */
    if (n > sz) {
        seq_t *tmp = (seq_t *)realloc(arr, sizeof(seq_t) * n);
        if (!tmp) return 0; /* allocation failure; give up */
        arr = tmp;
        /* Note: we keep sz unchanged until we have filled the new terms. */
    }

    /* Compute missing terms, starting from the current size up to n-1. */
    for (idx_t i = sz; i < n; ++i) { /* i indexes 0‑based array position */
        /* arr[i] corresponds to a(i+1).  Use the recurrence. */
        seq_t val = arr[i - 1]                     /* a(i)   */
                  + 2 * arr[i - 2]                 /* 2*a(i-1) */
                  + 3 * arr[i - 3]                 /* 3*a(i-2) */
                  + 2 * arr[i - 4]                 /* 2*a(i-3) */
                  + arr[i - 5];                    /* a(i-4)  */
        arr[i] = val;
    }

    /* Update the cached size to reflect the newly known terms. */
    sz = n;

    return arr[n - 1];
}
