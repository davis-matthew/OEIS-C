/*
 * A343352  Expansion of Product_{k>=1} 1 / (1 - x^k)^(7^(k-1)).
 * https://oeis.org/A343352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1‑x^k)^{7^{k‑1}}
 *            Recurrence: a(0)=1, a(n)= (1/n) * Σ_{k=1..n} ( Σ_{d|k} d·7^{d‑1}
 *            ) · a(n‑k), n≥1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *coeff = NULL;          /* a(n) values          */
static size_t  cached_n = 0;         /* number of cached entries (max index+1) */

/* ---------------------------------------------------------------------- */
/*                       C++ (big‑int) implementation                     */
/* ---------------------------------------------------------------------- */
#if defined(__cplusplus)

static seq_t *weight = NULL;          /* weight[d] = 7^{d-1}   */
static seq_t *sdiv   = NULL;          /* sdiv[k]   = Σ_{d|k} d·weight[d] */

seq_t a(idx_t n)
{
    size_t new_size = (size_t)n + 1;                 /* indices 0..n inclusive */
    if (new_size > cached_n) {
        size_t old = cached_n;
        coeff   = (seq_t *)realloc(coeff,   new_size * sizeof(seq_t));
        weight  = (seq_t *)realloc(weight,  new_size * sizeof(seq_t));
        sdiv    = (seq_t *)realloc(sdiv,   new_size * sizeof(seq_t));
        if (!coeff || !weight || !sdiv) abort();

        /* zero‑initialize new slots */
        for (size_t i = old; i < new_size; ++i) {
            coeff[i] = 0;
            weight[i] = 0;
            sdiv[i] = 0;
        }
        if (old == 0) coeff[0] = 1;                /* a(0) = 1 */

        /* compute weight[d] = 7^{d-1} */
        if (new_size >= 2) {
            weight[1] = (seq_t)1;                  /* 7^0 */
            for (size_t i = 2; i < new_size; ++i)
                weight[i] = weight[i-1] * (seq_t)7;
        }

        /* compute sdiv[k] = Σ_{d|k} d·weight[d] */
        for (size_t k = 0; k < new_size; ++k) sdiv[k] = 0;
        for (size_t d = 1; d < new_size; ++d) {
            seq_t add = (seq_t)d * weight[d];
            for (size_t m = d; m < new_size; m += d)
                sdiv[m] += add;
        }

        /* recurrence for a[i] */
        for (size_t i = 1; i < new_size; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= i; ++k)
                sum += sdiv[k] * coeff[i - k];
            coeff[i] = sum / (seq_t)i;              /* division exact */
        }
        cached_n = new_size;
    }
    return coeff[(size_t)n];
}

/* ---------------------------------------------------------------------- */
#else
/* ---------------------------------------------------------------------- */
/*                           C (uint64_t) implementation                  */
/* ---------------------------------------------------------------------- */
static uint64_t   *coeff64 = NULL;   /* a(n) values  */
static unsigned __int128 *weight128 = NULL;  /* weight[d]   */
static unsigned __int128 *sdiv128   = NULL;  /* sdiv[k]     */
static size_t  cached_n64 = 0;

seq_t a(idx_t n)
{
    size_t new_size = (size_t)n + 1;                   /* indices 0..n */
    if (new_size > cached_n64) {
        size_t old = cached_n64;
        coeff64   = (uint64_t *)realloc(coeff64,   new_size * sizeof(uint64_t));
        weight128 = (unsigned __int128 *)realloc(weight128,
                                                  new_size * sizeof(unsigned __int128));
        sdiv128   = (unsigned __int128 *)realloc(sdiv128,
                                                  new_size * sizeof(unsigned __int128));
        if (!coeff64 || !weight128 || !sdiv128) abort();

        /* zero‑initialize new slots */
        for (size_t i = old; i < new_size; ++i) {
            coeff64[i] = 0;
            weight128[i] = 0;
            sdiv128[i] = 0;
        }
        if (old == 0) coeff64[0] = 1;                 /* a(0) = 1 */

        /* weight[d] = 7^{d-1}   */
        if (new_size >= 2) {
            weight128[1] = (unsigned __int128)1;       /* 7^0 */
            for (size_t i = 2; i < new_size; ++i)
                weight128[i] = weight128[i-1] * (unsigned __int128)7;
        }

        /* sdiv[k] = Σ_{d|k} d·weight[d] */
        for (size_t k = 0; k < new_size; ++k) sdiv128[k] = 0;
        for (size_t d = 1; d < new_size; ++d) {
            unsigned __int128 add = (unsigned __int128)d * weight128[d];
            for (size_t m = d; m < new_size; m += d)
                sdiv128[m] += add;
        }

        /* recurrence for a[i] */
        for (size_t i = 1; i < new_size; ++i) {
            unsigned __int128 sum = 0;
            for (size_t k = 1; k <= i; ++k)
                sum += sdiv128[k] * coeff64[i - k];
            coeff64[i] = (uint64_t)(sum / (unsigned __int128)i);   /* division exact */
        }
        cached_n64 = new_size;
    }
    return (seq_t)coeff64[(size_t)n];
}
#endif
