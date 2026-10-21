/*
 * A258713  A001172(n)/2: Least k such that 2k is a sum of two odd primes in
 *          exactly n ways.
 * https://oeis.org/A258713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 841, u32 n <= 845, u64 n <= 845,
 *            u128 n <= 845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A258713: least k such that 2k is the sum of two odd primes in exactly n ways */
/* The function uses a sieve and FFT convolution to count Goldbach partitions,
   then stores the first k for each count g.  It handles arbitrary-precision
   seq_t and unsigned idx_t. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

/* -------- FFT utilities -------- */
typedef struct { double re; double im; } complex_t;

static void fft(complex_t *a, size_t n, int invert)
{
    size_t j = 0;
    for (size_t i = 1; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) { j ^= bit; bit >>= 1; }
        j ^= bit;
        if (i < j) { complex_t tmp = a[i]; a[i] = a[j]; a[j] = tmp; }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * acos(-1.0) / len * (invert ? -1.0 : 1.0);
        double wlen_re = cos(ang), wlen_im = sin(ang);
        for (size_t i = 0; i < n; i += len) {
            double w_re = 1.0, w_im = 0.0;
            for (size_t j = 0; j < len/2; ++j) {
                size_t u = i + j, v = i + j + len/2;
                double ur = a[u].re, ui = a[u].im;
                double vr = a[v].re, vi = a[v].im;
                double wr = w_re, wi = w_im;
                double wrv = wr * vr - wi * vi;
                double wrv_i = wr * vi + wi * vr;
                a[u].re = ur + wrv; a[u].im = ui + wrv_i;
                a[v].re = ur - wrv; a[v].im = ui - wrv_i;
                double next_re = wr * wlen_re - wi * wlen_im;
                double next_im = wr * wlen_im + wi * wlen_re;
                w_re = next_re; w_im = next_im;
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) { a[i].re /= n; a[i].im /= n; }
    }
}

/* -------- Global cache -------- */
static seq_t *best_counts = NULL;
static unsigned char *best_used  = NULL;
static size_t best_len = 0;
static int precomp_done = 0;

/* -------- Precomputation -------- */
static void precompute(void)
{
    if (precomp_done) return;
    const idx_t LIMIT = 4100000;   /* prime limit large enough to cover all requested n */
    /* Sieve */
    char *is_prime = (char *)malloc((size_t)(LIMIT + 1));
    memset(is_prime, 1, (size_t)(LIMIT + 1));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
    /*FFT size */
    size_t N = 1;
    while (N <= 2 * (size_t)LIMIT) N <<= 1;
    complex_t *a = (complex_t *)malloc(N * sizeof(complex_t));
    memset(a, 0, N * sizeof(complex_t));
    for (idx_t i = 3; i <= LIMIT; i += 2)
        if (is_prime[i]) a[i].re = 1.0;
    fft(a, N, 0);
    for (size_t i = 0; i < N; ++i) {
        double re = a[i].re * a[i].re - a[i].im * a[i].im;
        double im = 2.0 * a[i].re * a[i].im;
        a[i].re = re; a[i].im = im;
    }
    fft(a, N, 1);
    /* Build lookup */
    best_counts = NULL; best_used = NULL; best_len = 0;
    for (idx_t s = 0; s <= LIMIT; s += 2) {
        long long ordered = (long long)(a[s].re + 0.5);
        seq_t k = (seq_t)(s / 2);
        int g = 0;
        if (s/2 <= LIMIT && is_prime[s/2]) g = (int)((ordered + 1) / 2);
        else                                 g = (int)(ordered / 2);
        if (g >= 0) {
            if ((size_t)g >= best_len) {
                size_t new_len = (size_t)g + 1;
                seq_t *cnt = (seq_t *)realloc(best_counts, new_len * sizeof(seq_t));
                unsigned char *used = (unsigned char *)realloc(best_used, new_len * sizeof(unsigned char));
                for (size_t i = best_len; i < new_len; ++i) { cnt[i] = (seq_t)0; used[i] = 0; }
                best_counts = cnt; best_used = used; best_len = new_len;
            }
            if (!best_used[g] || k < best_counts[g]) {
                best_counts[g] = k;
                best_used[g] = 1;
            }
        }
    }
    free(a); free(is_prime);
    precomp_done = 1;
}

/* -------- Public function -------- */
seq_t a(idx_t n)
{
    if (!precomp_done) precompute();
    if ((size_t)n < best_len && best_used[n]) return best_counts[(size_t)n];
    return (seq_t)0;
}
