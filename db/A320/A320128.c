/*
 * A320128  Number of integer solutions to a^2 + b^2 + 2*c^2 + 7*d^2 = n.
 * https://oeis.org/A320128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the integer solutions of a² + b² + 2c² + 7d² = n.
 *            The implementation loops over d and c, then uses the classical
 *            formula r₂(m) = 4·(d₁(m) – d₃(m)) to count the (a,b) pairs for
 *            the remaining value m = n – 7d² – 2c². The solution count for d
 *            and c includes both signs when the variable i...
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

/* -------------------------------------------------------------------- */
/*   Helper: build the array that stores r₂(m) for 0 ≤ m ≤ max_n         */
/* -------------------------------------------------------------------- */
static seq_t *r2_vals = NULL;   /* array of r₂(v) */
static size_t r2_cap = 0;       /* current capacity (size) */

static void ensure_r2(size_t max_n)
{
    if (max_n < r2_cap) return;   /* already built */

    size_t new_cap = max_n + 1;    /* include index max_n */
    r2_vals = (seq_t *)realloc(r2_vals, new_cap * sizeof(seq_t));
    if (!r2_vals) abort();        /* out of memory */

    for (size_t m = r2_cap; m <= max_n; ++m) {
        if (m == 0) {
            r2_vals[m] = 1;        /* only (0,0) */
            continue;
        }
        seq_t diff = 0;            /* d₁(m) – d₃(m) */

        for (size_t d = 1; d * d <= m; ++d) {
            if (m % d != 0) continue;
            size_t d1 = d;
            size_t d2 = m / d;

            /* process divisor d1 */
            if (d1 % 4 == 1) diff += 1;
            else if (d1 % 4 == 3) diff -= 1;

            /* process divisor d2 if distinct */
            if (d2 != d1) {
                if (d2 % 4 == 1) diff += 1;
                else if (d2 % 4 == 3) diff -= 1;
            }
        }
        r2_vals[m] = (seq_t)(4 * diff);
    }
    r2_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/*   Main sequence routine                                                */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Build r₂(·) values up to n */
    ensure_r2((size_t)n);

    seq_t total = 0;

    /* Loop over d (≥ 0, integer) with 7*d² ≤ n */
    for (idx_t d = 0; ; ++d) {
        seq_t d2 = (seq_t)7 * d * d;
        if (d2 > n) break;
        seq_t wd = (d == 0) ? 1 : 2;   /* weight for d */

        /* Remaining value after d-term */
        seq_t rem_after_d = n - d2;

        /* Loop over c (≥ 0, integer) with 2*c² ≤ rem_after_d */
        for (idx_t c = 0; ; ++c) {
            seq_t c2 = (seq_t)2 * c * c;
            if (c2 > rem_after_d) break;
            seq_t wc = (c == 0) ? 1 : 2;   /* weight for c */

            idx_t rem = (idx_t)(rem_after_d - c2);  /* remaining for a² + b² */

            seq_t ways_ab = r2_vals[rem];           /* r₂(rem) */

            total += wd * wc * ways_ab;
        }
    }
    return total;
}
