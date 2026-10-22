/*
 * A367715  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - x * (1 + x + x^2 +
 *          x^3) * A(x^4))).
 * https://oeis.org/A367715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} a(floor(k/4)) * a(n-1-k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;   /* array of computed values */
static size_t  a_cap   = 0;      /* allocated capacity */
static size_t  a_len   = 0;      /* number of values currently stored */

/* Ensure that a_cache contains values up to index n (inclusive). */
static void a_ensure(idx_t n)
{
    if (a_len > n) return;                 /* already have enough */

    /* Grow capacity if needed */
    if (a_cap == 0) a_cap = 8;
    while (a_cap <= (size_t)n) a_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* memory allocation failed */
    a_cache = tmp;

    /* Compute new values */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)1;
            idx_t end = (idx_t)i - 1;       /* n-1 for this term */
            for (idx_t k = 0; k <= end; ++k) {
                idx_t f = k >> 2;           /* floor(k/4) */
                idx_t j = end - k;
                sum += a_cache[f] * a_cache[j];
            }
            a_cache[i] = sum;
        }
    }
    a_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    a_ensure(n);
    return a_cache[(size_t)n];
}
