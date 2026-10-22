/*
 * A352412  E.g.f.: 2*x / LambertW( 2*x/(1-x) ).
 * https://oeis.org/A352412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined by the e.g.f. A(x) = 2*x / LambertW(2*x/(1-x))
 *            The coefficients a(n) are computed by the recurrence u(n) =
 *            2*n*v(n‑1) e(n) = Σ_{k=0}^{n‑1} C(n‑1,k) e(k) u(n‑k) a(n) = e(n)
 *            – n e(n‑1) (a(0)=1) v(n) = –a(n) – Σ_{k=1}^{n‑1} C(n,k) a(k)
 *            v(n‑k) All sums use ordinary binomial coefficien...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;          /* a(n)  */
static seq_t *v_cache = NULL;          /* 1/A series */
static seq_t *u_cache = NULL;          /* 2*x/A series */
static seq_t *e_cache = NULL;          /* exp(2*x/A) series */
static size_t cache_cap = 0;           /* array capacity */
static size_t cache_size = 0;          /* number of terms computed (≥ 1) */

/* Allocate space for at least 'lim' coefficients (indices 0 … lim‑1). */
static void ensure_capacity(size_t lim)
{
    if (lim <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < lim) new_cap <<= 1;

    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    v_cache = (seq_t *)realloc(v_cache, new_cap * sizeof(seq_t));
    u_cache = (seq_t *)realloc(u_cache, new_cap * sizeof(seq_t));
    e_cache = (seq_t *)realloc(e_cache, new_cap * sizeof(seq_t));

    /* initialise new slots */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_cache[i] = 0;
        v_cache[i] = 0;
        u_cache[i] = 0;
        e_cache[i] = 0;
    }
    /* do NOT change cache_size here; it represents the number of
       already computed terms and will be updated by the caller. */
    cache_cap = new_cap;
}

/* Initialise the first term (n = 0). */
static void init_once(void)
{
    if (a_cache) return;              /* already initialised */
    ensure_capacity(8);
    a_cache[0] = 1;
    v_cache[0] = 1;
    u_cache[0] = 0;
    e_cache[0] = 1;
    cache_size = 1;
}

seq_t a(idx_t n)
{
    init_once();

    /* Extend cache up to n */
    if ((size_t)n >= cache_size) {
        size_t target = (size_t)n + 1;
        ensure_capacity(target);
        for (size_t i = cache_size; i < target; ++i) {
            /* 1. u(i) = 2*i*v(i-1)   (i>=1) */
            u_cache[i] = (seq_t)2 * (seq_t)i * v_cache[i-1];

            /* 2. e(i) = Σ_{j=0}^{i-1} C(i-1,j)·e(j)·u(i-j)   (i>=1) */
            seq_t sum_e = 0;
            seq_t binom = 1;                    /* C(i-1,0) */
            for (idx_t j = 0; j < i; ++j) {
                sum_e += binom * e_cache[j] * u_cache[i-j];
                if (j + 1 < i) {
                    seq_t num = (seq_t)(i - 1 - j);
                    seq_t den = (seq_t)(j + 1);
                    binom = binom * num / den;  /* exact division */
                }
            }
            e_cache[i] = sum_e;

            /* 3. a(i) = e(i) – i·e(i-1)   (i>=1) */
            a_cache[i] = e_cache[i] - (seq_t)i * e_cache[i-1];

            /* 4. v(i) :  v(0)=1 already set
                    v(i) = –a(i) – Σ_{k=1}^{i-1} C(i,k)·a(k)·v(i-k)   (i>=1) */
            if (i == 1) {
                v_cache[i] = - a_cache[i];
            } else {
                seq_t sum_v = 0;
                seq_t binomv = (seq_t)i;             /* C(i,1) */
                sum_v += binomv * a_cache[1] * v_cache[i-1];
                for (idx_t k = 2; k <= i-1; ++k) {
                    seq_t num = (seq_t)(i - (k - 1));
                    seq_t den = (seq_t)k;
                    binomv = binomv * num / den;    /* exact division */
                    sum_v += binomv * a_cache[k] * v_cache[i-k];
                }
                v_cache[i] = - a_cache[i] - sum_v;
            }
        }
        cache_size = target;
    }

    return a_cache[(size_t)n];
}
