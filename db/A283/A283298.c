/*
 * A283298  Diagonal of the Euler-Seidel matrix for the Catalan numbers.
 * https://oeis.org/A283298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} binomial(n,i) * A000108(n+i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

static seq_t *cat_cache = NULL;     /* cache of Catalan numbers C[0..k]   */
static idx_t   cat_size  = 0;       /* number of stored entries           */

/* Ensure Catalan numbers up to idx are computed and stored. */
static void
ensure_catalan(idx_t idx)
{
    if (cat_size > idx)
        return;                               /* already have them */

    idx_t new_size = idx + 1;

    if (cat_cache == NULL) {
        cat_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!cat_cache) abort();
    } else {
        cat_cache = (seq_t *)realloc(cat_cache, sizeof(seq_t) * new_size);
        if (!cat_cache) abort();
    }

    for (idx_t k = cat_size; k <= idx; ++k) {
        if (k == 0) {
            cat_cache[0] = 1;                 /* C0 = 1 */
        } else {
            /* Ck = C(k-1) * (4k-2) / (k+1)   */
            seq_t prev  = cat_cache[k - 1];
            seq_t numer = prev * ((seq_t)(4 * k - 2));
            seq_t denom = (seq_t)(k + 1);
            cat_cache[k] = numer / denom;
        }
    }
    cat_size = new_size;
}

/* a(n) = Σ_{i=0}^n binomial(n,i)*Catalan(n+i)   */
seq_t a(idx_t n)
{
    /* we need Catalan numbers up to C(2n) */
    ensure_catalan(2 * n);

    seq_t sum  = 0;
    seq_t binom= 1;             /* binomial(n,0) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t term = binom * cat_cache[n + i];
        sum += term;

        if (i < n) {
            /* binomial(n,i+1) = binomial(n,i)*(n-i)/(i+1) */
            seq_t num = binom * ((seq_t)(n - i));
            seq_t den = (seq_t)(i + 1);
            binom = num / den;
        }
    }
    return sum;
}
