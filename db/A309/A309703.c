/*
 * A309703  G.f. A(x) satisfies: A(x) = A(x^2) / (1 - x - x^2 - x^3 - x^4).
 * https://oeis.org/A309703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies A(x) = A(x^2) / (1 - x - x^2 - x^3 - x^4)
 *            Recurrence derived from the generating function: a(n) =
 *            Σ_{m=0..⌊n/2⌋} a(m) * b(n-2m) where b(0)=1 and
 *            b(k)=b(k-1)+b(k-2)+b(k-3)+b(k-4) for k≥1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms */
static seq_t *a_cache = NULL;   /* a[0..a_cap-1] */
static idx_t  a_cap  = 0;
static seq_t *b_cache = NULL;   /* b[0..b_cap-1] */
static idx_t  b_cap  = 0;

/* Extend the cached arrays up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n < a_cap)
        return;  /* already have enough */

    idx_t new_cap = n + 1;

    /* Reallocate the arrays.  realloc handles NULL pointers. */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));

    /* Compute missing b values using its 4‑term recurrence. */
    for (idx_t i = b_cap; i < new_cap; ++i) {
        if (i == 0) {
            b_cache[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            if (i >= 1) sum += b_cache[i - 1];
            if (i >= 2) sum += b_cache[i - 2];
            if (i >= 3) sum += b_cache[i - 3];
            if (i >= 4) sum += b_cache[i - 4];
            b_cache[i] = sum;
        }
    }

    /* Compute missing a values using the convolution with even indices. */
    for (idx_t i = a_cap; i < new_cap; ++i) {
        if (i == 0) {          /* a(0) = 1 */
            a_cache[0] = (seq_t)1;
            continue;
        }
        seq_t total = 0;
        for (idx_t m = 0; m <= i / 2; ++m) {
            total += a_cache[m] * b_cache[i - 2 * m];
        }
        a_cache[i] = total;
    }

    a_cap = new_cap;
    b_cap = new_cap;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[n];
}
