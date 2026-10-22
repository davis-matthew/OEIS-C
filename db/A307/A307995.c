/*
 * A307995  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 + x^4 + x^5 * (1 + A(x)
 *          + A(x^2) + A(x^3) + ...).
 * https://oeis.org/A307995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307995:  a(1) = … = a(5) = 1;  for n ≥ 6,
   a(n) = Σ_{d | (n‑5)} a(d)   (i.e. a(n) is the divisor‐sum of a(n‑5))
   This implementation recomputes the required values from scratch
   for every call.  The algorithm is O(n log n), which easily fits
   the 2‑second limit for n up to about 10 000. */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 5) {
        return (seq_t)1;
    }

    /* Allocate array for 0 … n (index 0 unused) */
    seq_t *arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!arr) {
        /* Allocation failed – return 0 (unlikely for small n) */
        return (seq_t)0;
    }

    /* Initialise all entries to 0 */
    for (idx_t i = 0; i <= n; ++i) {
        arr[i] = (seq_t)0;
    }

    /* Set first five values to 1 */
    for (idx_t i = 1; i <= 5; ++i) {
        arr[i] = (seq_t)1;
    }

    /* Sieve‑like computation of divisor sums for positions
       corresponding to n‑5.  Only indices up to n‑5 need be
       processed as they contribute to indices 6 … n. */
    idx_t limit = n - 5;
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t val = arr[d];   /* current value of a(d) is already correct */
        for (idx_t m = d; m <= limit; m += d) {
            arr[m + 5] += val;   /* add a(d) to a(m+5) */
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
