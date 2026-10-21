/*
 * A256598  Irregular triangle where row n contains the odd terms in the
 *          Collatz sequence beginning with 2n+1.
 * https://oeis.org/A256598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term of the flattened triangle consisting of *
 *            the odd terms of the Collatz sequence starting at 2k+1. * The
 *            recurrence used is: * T(k,0) = 2k+1 * T(k,j+1) =
 *            odd_part(3*T(k,j)+1) * The function generates rows on demand,
 *            memoising all earlier rows * in a single flat array. It work...
 * Range:     u8 n <= 64, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/odd_part.h"

/* flat storage of all triangle terms */
static seq_t *tri_flat   = NULL;
/* number of terms currently stored */
static size_t tri_len    = 0;
/* allocated capacity of tri_flat */
static size_t tri_cap    = 0;
/* number of rows fully generated */
static idx_t  rows_done  = 0;

/* Ensure the flat array can hold at least "need" elements. */
static void
ensure_capacity(size_t need)
{
    if (tri_cap >= need) return;
    size_t new_cap = tri_cap ? tri_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;

    /* Use reinterpret_cast for C++ and cast for C. */
    seq_t *p = (seq_t *)realloc(tri_flat, new_cap * sizeof(seq_t));
    if (!p) abort();          /* insufficient memory */
    tri_flat = p;
    tri_cap  = new_cap;
}

/* Generate the next row of the triangle and append its odd terms to tri_flat. */
static void
generate_next_row(void)
{
    seq_t start = (seq_t)rows_done * 2 + (seq_t)1; /* 2*r+1 */

    seq_t val = start;
    for (;;)
    {
        ensure_capacity(tri_len + 1);
        tri_flat[tri_len++] = val;
        if (val == (seq_t)1) break;
        val = odd_part((seq_t)3 * val + (seq_t)1);
    }
    rows_done++;
}

/* Public API: returns the n-th term of the flattened triangle. */
seq_t a(idx_t n)
{
    while (tri_len <= (size_t)n)
        generate_next_row();
    return tri_flat[(size_t)n];
}
