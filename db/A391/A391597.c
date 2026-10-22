/*
 * A391597  a(n) is the number of quadratic polynomials with coefficients in
 *          {-n, ..., n}, positive leading coefficient, and having two integer
 *          roots.
 * https://oeis.org/A391597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{u=1..n} b(floor(n/u)) where b(m) is the number of
 *            unordered integer pairs (r,s) with r ≤ s, |r+s| ≤ m and |r*s| ≤
 *            m
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   count_pairs_for_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_pairs_for_m.h"

/* Cache for the b(m) values */
static seq_t *b_cache = NULL;   /* b_cache[k] = b(k) for 0 ≤ k ≤ b_cap */
static idx_t b_cap   = 0;

/* Ensure b_cache[0..up_to] is properly filled */
static void ensure_b_cache(idx_t up_to)
{
    if (up_to <= b_cap) return;

    seq_t *new_cache;
    if (b_cap == 0) {
        new_cache = (seq_t *)malloc((size_t)(up_to + 1) * sizeof(seq_t));
        if (!new_cache) return;              /* allocation failure – unlikely */
        new_cache[0] = (seq_t)0;
    } else {
        new_cache = (seq_t *)realloc(b_cache,
                                     (size_t)(up_to + 1) * sizeof(seq_t));
        if (!new_cache) return;
    }

    for (idx_t k = b_cap + 1; k <= up_to; ++k)
        new_cache[k] = count_pairs_for_m(k);

    b_cache = new_cache;
    b_cap   = up_to;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_b_cache(n);

    seq_t sum = 0;
    for (idx_t u = 1; u <= n; ++u) {
        idx_t m = n / u;           /* floor(n/u) */
        sum += b_cache[m];
    }
    return sum;
}
