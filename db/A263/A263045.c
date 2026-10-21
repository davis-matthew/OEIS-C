/*
 * A263045  a(1)=a(2)=1, a(3)=2; for n>3, a(n) = (a(n-1) + a(n-2))*a(n-3) -
 *          a(n-1).
 * https://oeis.org/A263045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2; for n>3, a(n) = (a(n-1) + a(n-2))*a(n-3)
 *            - a(n-1) (recurrence)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 14, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised storage of the sequence.  The array is one-based in logic
   (a(1) stored at index 0). */
static seq_t *a_cache = NULL;   /* array of computed values          */
static size_t a_len   = 0;       /* number of stored values           */
static size_t a_cap   = 0;       /* current allocation capacity       */
static int    a_inited = 0;      /* guard for first initialisation    */

/* Ensure the cache can hold up to the given index n.
   Expands the array lazily and computes any missing terms. */
static void a_ensure(idx_t n)
{
    if (n <= a_len) return;            /* already computed */

    /* Expand the array as needed.  Use doubling growth for efficiency. */
    size_t new_cap = a_cap ?: 1;
    while (new_cap < (size_t)n) new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_cache;
    a_cap   = new_cap;

    /* Compute missing values from a_len+1 up to n. */
    for (idx_t i = a_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1 || i == 2) {
            val = 1;
        } else if (i == 3) {
            val = 2;
        } else {
            seq_t a1 = a_cache[i - 2]; /* a(i-1) */
            seq_t a2 = a_cache[i - 3]; /* a(i-2) */
            seq_t a3 = a_cache[i - 4]; /* a(i-3) */
            val = (a1 + a2) * a3 - a1;
        }
        a_cache[i - 1] = val;
    }
    a_len = n;
}

/* External function required by the harness. */
seq_t a(idx_t n)
{
    if (!a_inited) {        /* lazy initialisation of the cache */
        a_cache = NULL;
        a_len = a_cap = 0;
        a_inited = 1;
    }
    a_ensure(n);
    return a_cache[n - 1];
}
