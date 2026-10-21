/*
 * A259760  Triangle read by rows: T(n,k) is the number of partial idempotent
 *          mappings (of an n-chain) with breadth exactly k.
 * https://oeis.org/A259760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Each a(n) is the (row, col) entry of the triangle * T(n,k) =
 *            binomial(n,k) * Σ_{m=0..k} binomial(k,m) * m^(k-m), * read
 *            row‑wise. A memoised array holds the pre‑computed sums * for k
 *            and the function binomial(n,k) and pow_int(base,exp) are *
 *            assumed to be provided by the evaluation framework
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

static seq_t *S = NULL;          /* memoised Σ_{m=0..k} … values   */
static idx_t S_size = 0;         /* current allocated capacity      */
static idx_t S_computed = 0;     /* first index NOT yet computed   */

/* Ensure the cache holds values up to index 'upto' inclusive.
 * New entries are computed only once. */
static void ensure_S(idx_t upto)
{
    /* allocate / grow the array */
    if (!S || upto + 1 > S_size) {
        seq_t *tmp = (seq_t *)realloc(S, (upto + 1) * sizeof(seq_t));
        if (!tmp)
            return;                     /* catastrophic: caller will abort   */
        S = tmp;
        S_size = upto + 1;
    }

    /* compute any missing values */
    for (idx_t k = S_computed; k <= upto; ++k) {
        seq_t sum = 0;
        for (idx_t m = 0; m <= k; ++m) {
            seq_t bin = binomial(k, m);
            seq_t p   = pow_int((seq_t)m, k - m);
            sum += bin * p;
        }
        S[k] = sum;
    }
    if (upto + 1 > S_computed)
        S_computed = upto + 1;
}

/* Return the a(n) entry (triangle read by rows). */
seq_t a(idx_t n)
{
    /* Determine row and column of the nth triangle entry. */
    idx_t row = 0;
    idx_t cum = 0;
    while (cum + row + 1 <= n) {
        cum += row + 1;
        ++row;
    }
    idx_t col = n - cum;            /* 0‑based column in this row   */

    /* Ensure the needed Σ-values are ready. */
    ensure_S(col);

    /* Compute the triangle entry T(row,col). */
    seq_t bin   = binomial(row, col);
    seq_t result = bin * S[col];
    return result;
}
