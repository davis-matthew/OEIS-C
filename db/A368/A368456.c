/*
 * A368456  Number of independent vertex sets in the
 *          n-Dorogovtsev-Goltsev-Mendes graph.
 * https://oeis.org/A368456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368456: Number of independent vertex sets in the n-Dorogovtsev-Goltsev-Mendes graph.
   Recurrence:
     b(0) = 1, c(0) = 1
     c(n) = b(n-1) * c(n-1)^2
     b(n) = c(n) + b(n-1)^3
   a(n) = b(n) + 2*c(n)
*/

#include <stdlib.h>

static seq_t *b = NULL;
static seq_t *c = NULL;
static size_t max_alloc = 0;      /* size of allocated arrays              */
static size_t computed   = 0;      /* number of indices computed (next free) */

/* Ensure array capacity for index n (0‑based). */
static void ensure_capacity(idx_t n)
{
    if (b == NULL) {
        size_t cap = 1;
        while (cap <= (size_t)n) cap <<= 1;
        b = (seq_t *)malloc(cap * sizeof(seq_t));
        c = (seq_t *)malloc(cap * sizeof(seq_t));
        b[0] = (seq_t)1;
        c[0] = (seq_t)1;
        max_alloc = cap;
        computed  = 1;           /* we have b[0] and c[0] */
    } else if (max_alloc <= (size_t)n) {
        size_t cap = max_alloc;
        while (cap <= (size_t)n) cap <<= 1;
        b = (seq_t *)realloc(b, cap * sizeof(seq_t));
        c = (seq_t *)realloc(c, cap * sizeof(seq_t));
        max_alloc = cap;
    }
}

/* Update the recurrence from the first missing index up to n. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        if (i == 0) continue;          /* base is already set */

        /* c[i] = b[i-1] * c[i-1]^2 */
        seq_t temp = c[i-1];
        temp = temp * temp;            /* c[i-1]^2 */
        c[i] = b[i-1] * temp;

        /* b[i] = c[i] + b[i-1]^3 */
        seq_t t2 = b[i-1];
        t2 = t2 * t2;                  /* b[i-1]^2 */
        t2 = t2 * b[i-1];              /* b[i-1]^3 */
        b[i] = c[i] + t2;
    }
    computed = (size_t)n + 1;          /* indices 0..n are now computed */
}

/* Public function that returns the nth term. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    compute_up_to(n);
    seq_t result = b[n] + (c[n] << 1);
    return result;
}
