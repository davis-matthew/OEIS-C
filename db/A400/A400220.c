/*
 * A400220  Nondecreasing sequence of positive integers in which each k >= 1
 *          occurs exactly Sum_{d|k} a(d) times.
 * https://oeis.org/A400220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(1)=1; for n≥1, a(n+1)=a(n)+[ n==∑_{d=1..a(n)} a(d)*⌊a(n)/d⌋ ]
 * Range:     u8 n <= 9723, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for index 0 */

    /* Memoisation storage: 1‑based array of sequence terms */
    static seq_t *seq   = NULL;           /* array of values   */
    static idx_t  maxIdx = 0;             /* highest computed index */
    static size_t capacity = 0;           /* current array capacity */

    /* Ensure the array is large enough for indices up to n */
    if ((size_t)n + 1 > capacity) {
        size_t newCap = capacity ? capacity * 2 : 16;
        if ((size_t)n + 1 > newCap) newCap = (size_t)n + 1;

        /* Allocate new storage */
        seq_t *newArr = (seq_t *)malloc(newCap * sizeof(seq_t));
        if (!newArr) return (seq_t)0;    /* allocation failure */

        /* Copy existing values */
        for (size_t i = 0; i < capacity; ++i) newArr[i] = seq[i];

        /* Free old storage */
        if (seq) free(seq);

        seq = newArr;
        capacity = newCap;
    }

    /* Initialise the first term */
    if (maxIdx == 0) {
        seq[1] = (seq_t)1;
        maxIdx = 1;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = maxIdx + 1; i <= n; ++i) {
        seq_t const prev = seq[i - 1];

        /* S = ∑_{d=1..prev} a(d) * ⌊prev/d⌋ */
        seq_t S = (seq_t)0;
        idx_t limit = (idx_t)prev;            /* safe: prev fits in idx_t for n≤10000 */
        for (idx_t d = 1; d <= limit; ++d) {
            seq_t q = prev / (seq_t)d;
            S += seq[d] * q;
        }

        /* Determine a(i) */
        if ((seq_t)(i - 1) == S) {
            seq[i] = prev + 1;
        } else {
            seq[i] = prev;
        }
    }

    maxIdx = n;
    return seq[n];
}
