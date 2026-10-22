/*
 * A400746  G.f. A(x) satisfies: A(x) = 1 + x * A(2*x/(1 - 2*x)) / (1 - x)^2.
 * https://oeis.org/A400746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = n + Σ_{j=1}^{n-1} (n-j)*2^j * Σ_{k=1}^{j} C(j-1,k-1)*a(k)
 *            with a(0) = 1 This implementation uses the equivalent recurrence
 *            with an auxiliary array S[j] = Σ_{k=1}^{j} C(j-1,k-1)*a(k).
 *            Values are cached so that calls to a(n) in any order are O(n) in
 *            total
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* size_t, realloc */

/* Cached sequence entries and auxiliary tables */
static seq_t *a_seq   = NULL;   /* a[n]   */
static seq_t *S_seq   = NULL;   /* S[n]   */
static seq_t *pow2_seq= NULL;   /* 2^n    */
static size_t cache_sz   = 0;   /* allocated size of the arrays          */
static size_t computed_n = 0;   /* highest n already computed (≥0)       */
static size_t pow_sz     = 0;   /* number of powers of two known         */

/* Ensure the static arrays can hold indices up to 'need' (inclusive) */
static void ensure_capacity(size_t need)
{
    if (cache_sz > need)
        return;
    size_t new_sz = (cache_sz == 0) ? 1 : cache_sz;
    while (new_sz <= need)
        new_sz <<= 1;

    a_seq    = (seq_t *)realloc(a_seq,    new_sz * sizeof(seq_t));
    S_seq    = (seq_t *)realloc(S_seq,    new_sz * sizeof(seq_t));
    pow2_seq = (seq_t *)realloc(pow2_seq, new_sz * sizeof(seq_t));

    for (size_t i = cache_sz; i < new_sz; ++i) {
        a_seq   [i] = 0;
        S_seq   [i] = 0;
        pow2_seq[i] = 0;
    }
    cache_sz = new_sz;
}

/* Compute powers of two up to 'need' (inclusive).  pow2_seq[0] == 1. */
static void compute_powers(size_t need)
{
    if (pow_sz > need)
        return;
    if (pow_sz == 0) {
        pow2_seq[0] = (seq_t)1;
        pow_sz = 1;
    }
    for (size_t i = pow_sz; i <= need; ++i) {
        pow2_seq[i] = pow2_seq[i-1] * ((seq_t)2);
    }
    pow_sz = need + 1;
}

/* Return a(n).  The function is safe for any n >= 0 and supports
   arbitrary ordering of calls. */
seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n;

    ensure_capacity(N);
    if (a_seq[0] == 0) {
        a_seq[0] = (seq_t)1;
    }

    compute_powers(N);

    size_t start = computed_n + 1;
    if (start < 1)
        start = 1;

    for (size_t cur = start; cur <= N; ++cur) {
        /* a[cur] = cur + Σ_{j=1}^{cur-1} (cur-j)*2^j * S[j] */
        seq_t sum = (seq_t)cur;
        for (size_t j = 1; j < cur; ++j) {
            seq_t term = (seq_t)(cur - j);
            term *= pow2_seq[j];
            term *= S_seq[j];
            sum += term;
        }
        a_seq[cur] = sum;

        /* S[cur] = Σ_{k=1}^{cur} C(cur-1,k-1) * a[k] */
        seq_t S_val = 0;
        seq_t binom = 1;   /* C(cur-1,0) */
        for (size_t k = 1; k <= cur; ++k) {
            S_val += binom * a_seq[k];
            if (k < cur) {
                /* C(cur-1,k) = C(cur-1,k-1) * (cur-k) / k */
                binom = (binom * (seq_t)(cur - k)) / (seq_t)k;
            }
        }
        S_seq[cur] = S_val;
    }

    computed_n = N;
    return a_seq[N];
}
