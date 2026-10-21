/*
 * A242313  Numbers belonging to a geometric sequence whose ratio is 2 and
 *          whose first term ends in 1.
 * https://oeis.org/A242313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form 2^m · (10 k + 1) sorted increasingly.
   a(n) is the n-th smallest such number (n ≥ 1). */

#include <stdlib.h>
#include <stdint.h>

/* Comparator required by qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* 0 is a safe return for invalid index */

    /* A generous upper bound for a(n).  Empirically a(n) ≈ 5 n,
       so 12 n is comfortably safe for n ≤ 100 000. */
    seq_t limit = (seq_t)n * (seq_t)12;

    /* Estimate capacity needed: every base (ending in 1) contributes at
       most floor(log2(limit)) + 1 numbers.  We over‑allocate a bit. */
    size_t max_bases = (size_t)(limit / (seq_t)10) + 1;
    size_t max_log = 0;
    seq_t tmp = limit;
    while (tmp) { tmp >>= 1; ++max_log; }   /* floor(log2(limit)) + 1 */
    size_t cap = max_bases * max_log + 16;

    /* Allocate array to hold all candidates ≤ limit. */
    seq_t *arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!arr) return 0;                     /* allocation failure */

    size_t sz = 0;                          /* number of elements inserted */

    /* Generate all numbers of the form (10 k + 1)·2^m ≤ limit. */
    for (seq_t base = (seq_t)1; base <= limit; base += (seq_t)10) {
        seq_t val = base;
        while (val <= limit) {
            if (sz >= cap) {                /* safety: grow if needed */
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(arr, newcap * sizeof(seq_t));
                if (!tmp) { free(arr); return 0; }
                arr = tmp; cap = newcap;
            }
            arr[sz++] = val;
            if (val > limit >> 1) break;    /* avoid overflow on << */
            val <<= 1;                      /* multiply by 2 */
        }
    }

    /* Sort the generated numbers. */
    qsort(arr, sz, sizeof(seq_t), cmp_seq_t);

    /* Return the (n‑1)‑th element (since a(1) is the first term). */
    seq_t result = (n - 1 < sz) ? arr[n - 1] : (seq_t)0;

    free(arr);
    return result;
}
