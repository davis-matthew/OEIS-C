/*
 * A287635  a(1) = a(2) = a(3) = a(4) = 1; a(n) = a(a(n-4)) + a(n-a(n-3)) for
 *          n > 4.
 * https://oeis.org/A287635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(a(n-4)) + a(n-a(n-3)), a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 666, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *A = NULL;          /* 1‑based cache: A[i] = a(i) */
static size_t A_cap = 0;          /* allocated size (number of elements)   */
static idx_t  A_last = 0;         /* largest n for which a(n) is already computed */

/* Ensure the cache can hold indices up to n (inclusive).  Grow exponentially. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= A_cap)
        return;

    size_t new_cap = A_cap ? A_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                      /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* allocation failed */
    /* zero out the newly added elements */
    memset(tmp + A_cap, 0, (new_cap - A_cap) * sizeof(seq_t));
    A = tmp;
    A_cap = new_cap;
}

/* Initialise the first four terms once. */
static void init_first_terms(void)
{
    if (A_last >= 4) return;
    A[1] = A[2] = A[3] = A[4] = (seq_t)1;
    A_last = 4;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for 0 – return 0 */
        return (seq_t)0;

    if (!A) {                      /* lazy allocation on first use */
        ensure_capacity(n);
        init_first_terms();
    } else {
        ensure_capacity(n);
        if (A_last < 4) init_first_terms();
    }

    /* Compute any missing values up to n */
    for (idx_t i = A_last + 1; i <= n; ++i) {
        /* a(a(i-4)) */
        idx_t idx1 = (idx_t)A[i - 4];
        seq_t val1 = A[idx1];

        /* a(i - a(i-3)) */
        idx_t idx2 = i - (idx_t)A[i - 3];
        seq_t val2 = A[idx2];

        A[i] = val1 + val2;
    }
    A_last = n;
    return A[n];
}
