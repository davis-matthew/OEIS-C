/*
 * A395602  Square array A(n,k), n >= 2, k >= 1, read by antidiagonals
 *          downwards, where A(n,k) = [x^(n*k-2)] (1-x) * (1+x)^(n*k-4) *
 *          (Sum_{j=0..k} x^j)^n.
 * https://oeis.org/A395602
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term (n ≥ 2) in the array A(n,k), read by
 *            antidiagonals downwards. A(n,k) itself is given by the binomial
 *            sum below
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 41, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial, index_to_ak
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/index_to_ak.h"

/* ------------------------------------------------------------------ */
/*  Signed binomial: C(top,k) where top may be negative.
    Returns |C(top,k)| and writes sign = ±1. */
static seq_t signed_binom_abs_int64(int64_t top, idx_t k, int *sign)
{
    if (top >= 0) {
        *sign = 1;
        return binomial((idx_t)top, k);
    }
    /* C(-m , k) = (-1)^k * C(m + k - 1 , k) */
    idx_t m = (idx_t)(-top);
    *sign = (k & 1) ? -1 : 1;
    return binomial(m + k - 1, k);
}

/* ------------------------------------------------------------------ */
/*  Cache for A(n,k) values (n <= 199, k <= 199).  The array is
    large enough for indices arising from n up to about 10000. */
#define MAX_NK 200
static seq_t A_cache[MAX_NK][MAX_NK];
static unsigned char A_cached[MAX_NK][MAX_NK];

/*  Compute A(n,k) via the double sum formula and memoise. */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (n >= MAX_NK || k >= MAX_NK) return (seq_t)0;
    if (A_cached[n][k]) return A_cache[n][k];

    idx_t M = n * k;                /* n*k */
    idx_t i_max = M - 2;            /* last i value */

    seq_t posSum = 0, negSum = 0;   /* separate positive/negative parts */

    for (idx_t i = 0; i <= i_max; ++i) {
        int sign1;
        seq_t bin1 = signed_binom_abs_int64((int64_t)M - 4, i, &sign1);
        if (!bin1) continue;      /* term contributes 0 */

        /* inner sum over j */
        idx_t max_j = (M - i - 2) / (k + 1);
        seq_t innerPos = 0, innerNeg = 0;

        for (idx_t j = 0; j <= max_j; ++j) {
            seq_t bin_nj = binomial(n, j);
            if (!bin_nj) continue;

            int64_t top2 = (int64_t)n * (k + 1) - (int64_t)i - 4
                           - (int64_t)(k + 1) * j;
            int sign2;
            seq_t bin_top_abs = signed_binom_abs_int64(top2, n - 2, &sign2);
            if (!bin_top_abs) continue;

            seq_t term_abs = bin_nj * bin_top_abs;
            int sign_inner_j = sign2 * ((j & 1) ? -1 : 1);

            if (sign_inner_j >= 0)
                innerPos += term_abs;
            else
                innerNeg += term_abs;
        }

        /* absorption to outer sum */
        seq_t absInner = (innerPos >= innerNeg) ? (innerPos - innerNeg)
                                                : (innerNeg - innerPos);
        int signInner = (innerPos >= innerNeg) ? 1 : -1;

        seq_t term_abs_final = bin1 * absInner;
        int termSign = sign1 * signInner;

        if (termSign >= 0)
            posSum += term_abs_final;
        else
            negSum += term_abs_final;
    }

    seq_t result = (posSum >= negSum) ? (posSum - negSum) : (seq_t)0;
    A_cache[n][k] = result;
    A_cached[n][k] = 1;
    return result;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Cache for the final a(n) values so that repeated calls are fast. */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;          /* next index for which cache is valid */

/*  Compute a(n) from its index (n≥2). */
static seq_t single_a(idx_t index)
{
    if (index < 2) return (seq_t)0;

    idx_t n, k;
    index_to_ak(index, &n, &k);
    return compute_A(n, k);
}

/* ------------------------------------------------------------------ */
/*  Public function: returns a(n) for all n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    if (n >= a_cache_size) {
        /* grow cache up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_cache) { /* allocation failed; fall back to single computation */
            return single_a(n);
        }
        /* initialise new slots to 0 */
        for (idx_t i = a_cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
        /*
         * compute missing entries.  Since a_cache may have missing intermediate
         * indices that were not requested before, we compute all up to n.
         * Complexity is O(n^2), but for n ≤ 10000 and small cache size
         * (≈200) this is trivial.
         */
        for (idx_t i = a_cache_size; i <= n; ++i)
            new_cache[i] = single_a(i);
        a_cache = new_cache;
        a_cache_size = new_size;
    }
    return a_cache[n];
}
