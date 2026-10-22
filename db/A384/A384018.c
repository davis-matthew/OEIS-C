/*
 * A384018  a(n) = [x^n] Product_{k=0..n-1} (1 + k*x)^3.
 * https://oeis.org/A384018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Prod_{k=0..n-1} (1 + kx)^3 coefficients are
 *            computed by iterative convolution, keeping all polynomial terms
 *            up to degree 3n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* computed a(n) values */
static idx_t  cache_sz = 0;
static idx_t  max_computed = 0;   /* largest n already processed */

static seq_t *coeff = NULL;      /* coefficients of current polynomial */
static idx_t  coeff_sz = 0;      /* size of coeff array (degree + 1) */

static seq_t *temp = NULL;       /* temporary array during convolution */
static idx_t  temp_sz = 0;       /* size of temp array */

static void ensure_size(seq_t **arr, idx_t *sz, idx_t needed)
{
    if (needed * 3 + 1 > *sz) {
        seq_t *tmp = (seq_t *)realloc(*arr, (needed * 3 + 1) * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        for (idx_t i = *sz; i <= needed * 3; ++i) (*arr)[i] = 0;
        *sz = needed * 3 + 1;
    }
}

static void init(void)
{
    if (!cache) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache_sz = 1;
    }
    if (!coeff) {
        coeff = (seq_t *)calloc(1, sizeof(seq_t));
        if (!coeff) abort();
        coeff[0] = 1;
        coeff_sz = 1;
    }
    if (!temp) {
        temp = (seq_t *)calloc(1, sizeof(seq_t));
        if (!temp) abort();
        temp_sz = 1;
    }
    max_computed = 0;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;  /* safety for invalid index */
    init();

    if (n <= max_computed) return cache[n];

    ensure_size(&cache,  &cache_sz,  n);
    ensure_size(&coeff, &coeff_sz,  n);
    ensure_size(&temp,  &temp_sz,   n);

    /* cur_n is the number of factors already multiplied; product
       after this step has degree cur_deg = cur_n * 3. */
    idx_t cur_n = max_computed;      /* already processed */
    idx_t cur_deg = cur_n * 3;       /* current polynomial degree */

    while (cur_n < n) {
        idx_t k = cur_n;          /* next factor index */
        seq_t sk  = (seq_t)k;
        seq_t s3k = sk * (seq_t)3;            /* 3k */
        seq_t s3k2 = s3k * sk;                /* 3k^2 (to save work) */
        seq_t sk3  = sk * sk * sk;            /* k^3 */

        idx_t new_deg = cur_deg + 3;
        if (new_deg > n * 3) new_deg = n * 3;

        /* reset temp up to new_deg */
        for (idx_t i = 0; i <= new_deg; ++i) temp[i] = 0;

        /* convolution */
        for (idx_t t = 0; t <= cur_deg; ++t) {
            seq_t c = coeff[t];
            if (c == 0) continue;
            temp[t]     += c;                     /* *1 */
            if (t + 1 <= new_deg) temp[t + 1] += s3k * c;
            if (t + 2 <= new_deg) temp[t + 2] += s3k2 * c;
            if (t + 3 <= new_deg) temp[t + 3] += sk3 * c;
        }

        /* copy back to coeff */
        for (idx_t i = 0; i <= new_deg; ++i) coeff[i] = temp[i];

        cur_n   += 1;
        cur_deg  = new_deg;
        /* store a(cur_n) = coefficient of x^cur_n */
        cache[cur_n] = coeff[cur_n];
        if (cur_n + 1 > cache_sz) {
            cache = (seq_t *)realloc(cache, (cur_n + 1) * sizeof(seq_t));
            if (!cache) abort();
            cache_sz = cur_n + 1;
        }
    }

    max_computed = n;
    return cache[n];
}
