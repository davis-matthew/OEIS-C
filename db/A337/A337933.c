/*
 * A337933  Numbers that are the sum of two abundant numbers in exactly one
 *          way.
 * https://oeis.org/A337933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   next_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A337933: numbers that are the sum of two abundant
   numbers in exactly one way.  The algorithm builds an indicator array of
   abundant numbers up to a dynamic limit, performs an FFT convolution to
   count ordered representations, converts to unordered counts, and extracts
   those sums that have exactly one unordered representation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/next_pow2.h"

/* ---------- Basic complex type for FFT ---------- */
typedef struct { double re; double im; } Complex;

static inline Complex complex_add(Complex a, Complex b) {
    Complex r; r.re = a.re + b.re; r.im = a.im + b.im; return r;
}
static inline Complex complex_sub(Complex a, Complex b) {
    Complex r; r.re = a.re - b.re; r.im = a.im - b.im; return r;
}
static inline Complex complex_mul(Complex a, Complex b) {
    Complex r;
    r.re = a.re * b.re - a.im * b.im;
    r.im = a.re * b.im + a.im * b.re;
    return r;
}

/* Cooley–Tukey iterative FFT */
static void fft(Complex *a, size_t n, int invert) {
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) { j ^= bit; bit >>= 1; }
        j ^= bit;
        if (i < j) {
            Complex t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * acos(-1.0) / (double)len * (invert ? -1.0 : 1.0);
        Complex wlen = { cos(ang), sin(ang) };
        for (size_t i = 0; i < n; i += len) {
            Complex w = { 1.0, 0.0 };
            for (size_t j = 0; j < len / 2; ++j) {
                Complex u = a[i + j];
                Complex v = complex_mul(a[i + j + len / 2], w);
                a[i + j] = complex_add(u, v);
                a[i + j + len / 2] = complex_sub(u, v);
                w = complex_mul(w, wlen);
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            a[i].re /= (double)n;
            a[i].im /= (double)n;
        }
    }
}

/* ---------- Cached sequence storage ---------- */
static seq_t *cached_terms = NULL;   /* values of the sequence */
static size_t  cached_len   = 0;      /* number of cached terms   */
static uint32_t cached_lim = 0;      /* sieve limit used          */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined below the offset */

    /* If already enough terms cached, return directly. */
    if (cached_len >= (size_t)n) return cached_terms[(size_t)n - 1];

    /* Starting sieve limit; double until we accumulate enough terms. */
    uint32_t limit = cached_lim;
    if (limit == 0) limit = 200000;       /* initial guess */

    while (1) {
        /* 1. Sigma (sum of proper divisors) */
        uint64_t *sigma = (uint64_t*)calloc((size_t)(limit + 1), sizeof(uint64_t));
        if (!sigma) return (seq_t)0;          /* allocation failure */

        for (uint32_t i = 1; i <= limit; ++i) {
            for (uint32_t j = i << 1; j <= limit; j += i)
                sigma[j] += (uint64_t)i;
        }

        /* 2. Abundant indicator array */
        uint8_t *abundant = (uint8_t*)calloc((size_t)(limit + 1), sizeof(uint8_t));
        if (!abundant) { free(sigma); return (seq_t)0; }

        for (uint32_t i = 1; i <= limit; ++i)
            if (sigma[i] > (uint64_t)i) abundant[i] = 1;   /* correct abundant test */

        /* 3. Convolution via FFT */
        size_t fft_size = next_pow2((size_t)(2 * limit) + 1);
        Complex *fa = (Complex*)calloc(fft_size, sizeof(Complex));
        if (!fa) { free(sigma); free(abundant); return (seq_t)0; }

        for (uint32_t i = 0; i <= limit; ++i) {
            fa[i].re = abundant[i] ? 1.0 : 0.0;
            fa[i].im = 0.0;
        }
        fft(fa, fft_size, 0);                /* forward FFT */

        for (size_t i = 0; i < fft_size; ++i) {   /* square pointwise */
            Complex w = fa[i];
            fa[i].re = w.re * w.re - w.im * w.im;
            fa[i].im = 2.0 * w.re * w.im;
        }

        fft(fa, fft_size, 1);                /* inverse FFT */

        /* 4. Collect sums with exactly one unordered representation */
        size_t needed = (size_t)n;
        seq_t *new_terms = NULL;
        size_t new_len = 0, new_cap = 0;

        for (uint32_t s = 2; s <= limit; ++s) {
            long long ordered = (long long)(fa[s].re + 0.5); /* round to nearest */
            int same = ((s & 1) == 0 && abundant[s >> 1]) ? 1 : 0;
            long long unordered = (ordered + same) >> 1;      /* (ordered + same)/2 */

            if (unordered == 1) {
                if (new_len == new_cap) {
                    new_cap = new_cap ? new_cap << 1 : 64;
                    seq_t *tmp = (seq_t*)realloc(new_terms, new_cap * sizeof(seq_t));
                    if (!tmp) { /* memory failure: clean up */
                        free(new_terms);
                        free(fa);
                        free(abundant);
                        free(sigma);
                        return (seq_t)0;
                    }
                    new_terms = tmp;
                }
                new_terms[new_len++] = (seq_t)s;
            }
        }

        /* Clean up temporary arrays */
        free(fa);
        free(abundant);
        free(sigma);

        /* If enough terms collected, store them and return. */
        if (new_len >= needed) {
            if (cached_terms) free(cached_terms);
            cached_terms = new_terms;
            cached_len   = new_len;
            cached_lim   = limit;
            return cached_terms[(size_t)n - 1];
        }

        /* Not enough terms: discard and try a larger limit. */
        free(new_terms);
        limit <<= 1;          /* double the limit for next attempt */
    }
}
