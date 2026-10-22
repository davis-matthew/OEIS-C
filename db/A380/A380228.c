/*
 * A380228  Expansion of e.g.f. exp( exp( (exp(2*x)-1)/2 ) - 1 ).
 * https://oeis.org/A380228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * Stirling2(n,k) * Bell(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* a(n) values              */
static seq_t *bell_cache = NULL;   /* Bell numbers B_n         */
static seq_t *pow2_cache = NULL;   /* powers of 2              */
static seq_t *s_cache = NULL;      /* current Stirling2 row    */
static idx_t last_n = (idx_t)-1;   /* highest n computed       */
static size_t capacity = 0;        /* current array capacity   */

/* ensure all arrays have room for indices 0..n (inclusive) */
static void ensure_capacity(idx_t n) {
    size_t new_cap = (size_t)(n + 1);
    if (new_cap <= capacity)
        return;

    /* allocate or grow a_cache */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    /* allocate or grow bell_cache */
    tmp = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    bell_cache = tmp;

    /* allocate or grow pow2_cache */
    tmp = (seq_t *)realloc(pow2_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    pow2_cache = tmp;

    /* allocate or grow s_cache */
    tmp = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    s_cache = tmp;

    /* zero new entries if this is an expansion */
    if (capacity > 0) {
        for (size_t i = capacity; i < new_cap; ++i) {
            a_cache[i] = 0;
            bell_cache[i] = 0;
            pow2_cache[i] = 0;
            s_cache[i] = 0;
        }
    }

    capacity = new_cap;
}

/* compute all values up to n (inclusive) */
static void compute_up_to(idx_t n) {
    if (n == (idx_t)-1) abort();          /* guard against overflow   */
    if (last_n != (idx_t)-1 && n <= last_n)
        return;                           /* nothing to do            */

    ensure_capacity(n);

    /* initialise when starting from scratch */
    if (last_n == (idx_t)-1) {
        /* n = 0 base case */
        pow2_cache[0] = (seq_t)1;          /* 2^0 */
        bell_cache[0] = (seq_t)1;          /* B0  */
        s_cache[0]    = (seq_t)1;          /* S(0,0) */
        a_cache[0]    = (seq_t)1;          /* a(0)   */
        last_n = 0;
    }

    /* compute for n+1 .. desired n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* update Stirling2 row: S(i,k) = k*S(i-1,k)+S(i-1,k-1) */
        for (idx_t k = i; k > 0; --k) {
            s_cache[k] = (seq_t)k * s_cache[k] + s_cache[k - 1];
        }
        s_cache[0] = (seq_t)0;

        /* compute Bell number B_i using B_i = sum_{k=0}^{i-1} C(i-1,k)*B_k */
        seq_t sumB = (seq_t)0;
        seq_t binom = (seq_t)1;            /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            sumB += binom * bell_cache[k];
            if (k + 1 < i) {
                /* binom = C(i-1,k+1) = binom * (i-1-k)/(k+1) */
                seq_t num = (seq_t)(i - 1 - k);
                binom = (binom * num) / (k + 1);
            }
        }
        bell_cache[i] = sumB;

        /* compute power of two 2^i */
        pow2_cache[i] = pow2_cache[i - 1] << 1;

        /* compute a(i) = sum_{k=0..i} 2^(i-k) * S(i,k) * B_k */
        seq_t sumA = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term = pow2_cache[i - k];
            term *= s_cache[k];
            term *= bell_cache[k];
            sumA += term;
        }
        a_cache[i] = sumA;
    }

    last_n = n;
}

/* Public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n > (idx_t)~(size_t)0) abort();   /* guard against absurd size */
    compute_up_to(n);
    return a_cache[(size_t)n];
}
