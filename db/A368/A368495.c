/*
 * A368495  Psi-analog of the 5-core partition function.
 * https://oeis.org/A368495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Psi-analog of the 5-core partition function
 * G.f.:  ψ(-q^5)^5 / ψ(-q)
 * ψ(q)  = Σ_{k≥0} q^{k(k+1)/2}
 * ψ(-q) = Σ_{k≥0} (-1)^{k(k+1)/2} q^{k(k+1)/2}
 *
 * The function builds the two sparse power‑series
 *   f(q)=ψ(-q)      (coefficients at triangular indices)
 *   h(q)=ψ(-q^5)    (coefficients at indices 5·triangular)
 * and then computes g5(q)=h(q)^5 by repeated convolution.
 * Finally it performs series division g5(q)/f(q)
 * to obtain the desired coefficients a(n).
 * A static cache memoises all terms up to the largest
 * supplied index, so repeated calls (in any order) are fast.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache structure --------------------------------------- */
static struct {
    idx_t capacity;          /* maximum index + 1 that has been computed */
    seq_t *f;                /* coefficients of ψ(-q)          */
    seq_t *h;                /* coefficients of ψ(-q^5)        */
    seq_t *g5;               /* coefficients of (ψ(-q^5))^5    */
    seq_t *a;                /* final sequence a(n)            */
    idx_t *tri_f;            /* triangular indices for f      */
    size_t m_f;              /* number of such indices         */
    idx_t *tri_h;            /* 5×triangular indices for h    */
    size_t m_h;              /* number of such indices         */
} cache = {0, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0};

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache if necessary --------------------- */
    if (n + 1 > cache.capacity) {
        /* free old arrays if they exist -------------------------------- */
        free(cache.f);
        free(cache.h);
        free(cache.g5);
        free(cache.a);
        free(cache.tri_f);
        free(cache.tri_h);

        /* new size (index inclusive) ------------------------------------- */
        size_t cap = (size_t)(n + 1);
        cache.capacity = (idx_t)cap;

        /* allocate coefficient arrays --------------------------------- */
        cache.f  = (seq_t *)calloc(cap, sizeof(seq_t));
        cache.h  = (seq_t *)calloc(cap, sizeof(seq_t));
        cache.g5 = (seq_t *)calloc(cap, sizeof(seq_t));
        cache.a  = (seq_t *)calloc(cap, sizeof(seq_t));

        /* ---------- build ψ(-q) (f) ----------------------------------- */
        /* count triangular numbers <= n */
        size_t m_f = 0;
        for (idx_t k = 0;; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > n) break;
            ++m_f;
        }
        cache.m_f = m_f;
        cache.tri_f = (idx_t *)malloc(m_f * sizeof(idx_t));

        size_t idx = 0;
        for (idx_t k = 0;; ++k) {
            idx_t t = k * (k + 1) / 2;
            if (t > n) break;
            seq_t sign = ((t & 1) == 0) ? 1 : -1;
            cache.f[(size_t)t] = sign;
            if (t == 0) cache.f[0] = 1;  /* ensure f[0]==1 */
            cache.tri_f[idx++] = t;
        }

        /* ---------- build ψ(-q^5) (h) --------------------------------- */
        size_t m_h = 0;
        for (idx_t k = 0;; ++k) {
            idx_t t = 5 * k * (k + 1) / 2;
            if (t > n) break;
            ++m_h;
        }
        cache.m_h = m_h;
        cache.tri_h = (idx_t *)malloc(m_h * sizeof(idx_t));

        idx = 0;
        for (idx_t k = 0;; ++k) {
            idx_t t = 5 * k * (k + 1) / 2;
            if (t > n) break;
            idx_t tr = k * (k + 1) / 2;          /* triangular part for sign */
            seq_t sign = ((tr & 1) == 0) ? 1 : -1;
            cache.h[(size_t)t] = sign;
            cache.tri_h[idx++] = t;
        }

        /* ---------- convolution: compute g5 = h^5 --------------------- */
        seq_t *cur  = (seq_t *)calloc(cap, sizeof(seq_t));
        seq_t *next = NULL;
        cur[0] = 1;   /* start from 1 */

        for (int step = 0; step < 5; ++step) {
            next = (seq_t *)calloc(cap, sizeof(seq_t));
            for (size_t i = 0; i < cap; ++i) {
                if (cur[i] == 0) continue;
                idx_t iidx = (idx_t)i;
                for (size_t nt = 0; nt < cache.m_h; ++nt) {
                    idx_t t = cache.tri_h[nt];
                    idx_t j = iidx + t;
                    if (j >= (idx_t)cap) break;
                    next[(size_t)j] += cur[i] * cache.h[(size_t)t];
                }
            }
            free(cur);
            cur = next;
        }
        /* cur now holds g5 coefficients */
        for (size_t i = 0; i < cap; ++i) cache.g5[i] = cur[i];
        free(cur);

        /* ---------- division: compute a = g5 / f ---------------------- */
        cache.a[0] = cache.g5[0];  /* f[0]==1 */
        for (size_t i = 1; i < cap; ++i) {
            seq_t sum = 0;
            idx_t ii = (idx_t)i;
            for (size_t j = 1; j < cache.m_f; ++j) { /* skip t=0 */
                idx_t t = cache.tri_f[j];
                if (t > ii) break;
                sum += cache.f[(size_t)t] * cache.a[(size_t)(ii - t)];
            }
            cache.a[i] = cache.g5[i] - sum;
        }
    }
    /* cache now covers n ---------------------------------------------- */
    return cache.a[(size_t)n];
}
