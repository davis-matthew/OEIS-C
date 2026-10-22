/*
 * A327316  Triangular array read by rows: row n shows the coefficients of
 *          this polynomial of degree n: p(x,n) = ((x+r)^n - (x+s)^n)/(r - s),
 *          where r = 3 and s = 2.
 * https://oeis.org/A327316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k) * (3^(r-k) - 2^(r-k)) where r = row number
 *            such that (r-1)*r/2 < n ≤ r*(r+1)/2, and k = n - (r-1)*r/2 - 1
 *            (0‑based index within the row)
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t n, idx_t k) */

/* Cached powers of 3 and 2.  pow3[i] = 3^i, pow2[i] = 2^i. */
static seq_t *pow3 = NULL;
static seq_t *pow2 = NULL;
static size_t pow_cap = 0;

/* Ensure the power tables contain at least exp+1 entries.  exp is 0‑based. */
static void ensure_powers(idx_t exp)
{
    if ((size_t)exp + 1 <= pow_cap)
        return;

    size_t new_cap = (size_t)exp + 1;
    pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
    pow2 = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
    for (size_t i = pow_cap; i < new_cap; ++i) {
        if (i == 0) {
            pow3[0] = 1;
            pow2[0] = 1;
        } else {
            pow3[i] = pow3[i - 1] * 3;
            pow2[i] = pow2[i - 1] * 2;
        }
    }
    pow_cap = new_cap;
}

/* Return the n‑th term of A327316.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined, but harmless */

    /* Determine row r (1‑based) containing the term. */
    idx_t r = 1;
    while ( (idx_t)( (r + 1) * r / 2 ) < n )
        ++r;

    /* Index of the term within the row, 0‑based. */
    idx_t start = (r - 1) * r / 2 + 1;
    idx_t k = n - start;          /* 0 ≤ k < r */

    /* Exponent for the powers of 3 and 2. */
    idx_t exp = r - k;    /* 1 ≤ exp ≤ r */

    /* Make sure our power tables are large enough. */
    ensure_powers(exp);

    /* Compute the term value. */
    seq_t diff = pow3[exp] - pow2[exp];
    seq_t bin  = binomial((idx_t)r, (idx_t)k);
    return bin * diff;
}
