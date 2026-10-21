/*
 * A261667  Dimension of a certain space of duality relations arising in study
 *          of q-analogs of multiple zeta values.
 * https://oeis.org/A261667
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A000071(n) - A261666(n)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* G.F.: -x^6*(x^9 -x^2 -1)/((x-1)*(x^2 +x -1)*(x^14 +x^13 +x^12 +x^11 +x^10 +x^9 +x^8 -x^5 -x^4 -x^3 -x^2 +1)) */
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"

static const int c[18] = {
    0,   /* d0 placeholder */
    -2,  /* d1 */
    -1,  /* d2 */
    2,   /* d3 */
    1,   /* d4 */
    0,   /* d5 */
    1,   /* d6 */
    -1,  /* d7 */
    0,   /* d8 */
    -1,  /* d9 */
    -1,  /* d10 */
    0,   /* d11 */
    0,   /* d12 */
    0,   /* d13 */
    0,   /* d14 */
    -1,  /* d15 */
    1,   /* d16 */
    1    /* d17 */
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of elements already computed (indices 0 .. cache_len-1) */

static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

static void compute_to(idx_t n)
{
    if (cache_len > n)
        return;

    ensure_capacity(n);

    size_t prev_len = cache_len;
    for (idx_t i = (idx_t)prev_len; i <= n; ++i) {
        int coeff = 0;
        if (i == 6 || i == 8)
            coeff = 1;
        else if (i == 15)
            coeff = -1;

        seq_t sumNeg = 0;
        seq_t sumPos = 0;

        for (int k = 1; k <= 17; ++k) {
            if (i < (idx_t)k)
                continue;

            seq_t val = cache[i - k];
            int dk = c[k];
            if (dk < 0) {
                sumNeg += mul_small(val, (unsigned int)(-dk));
            } else if (dk > 0) {
                sumPos += mul_small(val, (unsigned int)dk);
            }
        }

        seq_t temp = sumNeg;
        if (coeff > 0)
            temp += (seq_t)coeff;
        else if (coeff < 0)
            temp -= (seq_t)(-coeff);

        temp -= sumPos;

        cache[i] = temp;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* values below offset are irrelevant */

    compute_to(n);
    return cache[n];
}
