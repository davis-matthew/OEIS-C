/*
 * A245625  Sequence of distinct least nonnegative numbers such that the
 *          average of the first n terms is a square.
 * https://oeis.org/A245625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction: a(n) is the smallest unused non‑negative integer a such that
   (∑_{i=1}^{n-1} a(i) + a) / n is a perfect square. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

#define MAX_N 20000          /* sufficient for n ≤ 10000 */

static seq_t terms[MAX_N];
static size_t term_len = 0;    /* number of terms computed so far */
static seq_t running_sum = 0;  /* Σ a(i) for i = 1 … term_len */

/* linear search for an already used value */
static bool used_value(seq_t v)
{
    size_t i;
    for (i = 0; i < term_len; ++i)
        if (terms[i] == v)
            return true;
    return false;
}

/* ceil of integer square root */
static seq_t ceil_sqrt(seq_t x)
{
    seq_t r = floor_sqrt(x);
    if (r * r < x)
        r++;
    return r;
}

/* compute the nth term, 1‑based */
seq_t a(idx_t n)
{
    if (n == 0)   /* out of bounds – undefined, return 0 */
        return 0U;

    /* compute items up to index n if necessary */
    while (term_len < n)
    {
        /* find minimal k such that new term is non‑negative and unused */
        seq_t t = (running_sum + (seq_t)n - 1) / (seq_t)n;
        seq_t k = ceil_sqrt(t);
        seq_t a_val;

        for (;; ++k)
        {
            a_val = (seq_t)n * k * k - running_sum;
            if (!used_value(a_val))
                break;
        }

        /* store the term and update state */
        terms[term_len] = a_val;
        ++term_len;
        running_sum += a_val;
    }

    return terms[n - 1];
}
