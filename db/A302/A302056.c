/*
 * A302056  Numbers k such that the coefficient of x^k in the expansion of
 *          Product_{j>=1} (1 - x^j)^4 is zero.
 * https://oeis.org/A302056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302056: a(n) = the n-th positive integer k such that the coefficient of x^k in the infinite product ∏_{j≥1}(1−x^j)^4 is zero. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Basic FFT helpers */
static const double PI = acos(-1.0);

static void fft(double *re, double *im, size_t n, int invert)
{
    /* Bit reversal */
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; (j & bit); bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            double tmp = re[i]; re[i] = re[j]; re[j] = tmp;
            tmp = im[i]; im[i] = im[j]; im[j] = tmp;
        }
    }
    /* Cooley–Tukey */
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * PI / len * (invert ? -1.0 : 1.0);
        double wlen_re = cos(ang), wlen_im = sin(ang);
        for (size_t i = 0; i < n; i += len) {
            double w_re = 1.0, w_im = 0.0;
            for (size_t j = 0; j < len / 2; ++j) {
                double u_re = re[i + j],   u_im = im[i + j];
                double v_re = re[i + j + len/2] * w_re - im[i + j + len/2] * w_im;
                double v_im = re[i + j + len/2] * w_im + im[i + j + len/2] * w_re;
                re[i + j]           = u_re + v_re;
                im[i + j]           = u_im + v_im;
                re[i + j + len/2]   = u_re - v_re;
                im[i + j + len/2]   = u_im - v_im;
                double next_w_re = w_re * wlen_re - w_im * wlen_im;
                double next_w_im = w_re * wlen_im + w_im * wlen_re;
                w_re = next_w_re; w_im = next_w_im;
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            re[i] /= (double)n;
            im[i] /= (double)n;
        }
    }
}

/* Integer convolution using FFT, result truncated to nOut */
static void convolution(const int64_t *a, size_t nA,
                        const int64_t *b, size_t nB,
                        int64_t *res, size_t nOut)
{
    size_t sz = 1;
    while (sz < nA + nB - 1) sz <<= 1;

    double *ar = (double*)malloc(sz * sizeof(double));
    double *ai = (double*)malloc(sz * sizeof(double));
    double *br = (double*)malloc(sz * sizeof(double));
    double *bi = (double*)malloc(sz * sizeof(double));

    for (size_t i = 0; i < nA; ++i) ar[i] = (double)a[i];
    for (size_t i = nA; i < sz; ++i) ar[i] = 0.0;
    for (size_t i = 0; i < nB; ++i) br[i] = (double)b[i];
    for (size_t i = nB; i < sz; ++i) br[i] = 0.0;

    /* imag parts already zero */

    fft(ar, ai, sz, 0);
    fft(br, bi, sz, 0);
    for (size_t i = 0; i < sz; ++i) {
        double rr = ar[i] * br[i] - ai[i] * bi[i];
        double ii = ar[i] * bi[i] + ai[i] * br[i];
        ar[i] = rr; ai[i] = ii;
    }
    fft(ar, ai, sz, 1);

    for (size_t i = 0; i < nOut; ++i) {
        res[i] = (int64_t)llround(ar[i]);   /* exact integer */
    }

    free(ar); free(ai); free(br); free(bi);
}

/* ------------------------------------------------------------------ */
/*                     Sequence implementation                           */
/* ------------------------------------------------------------------ */

static bool      initialized = false;
static idx_t   *zero_arr    = NULL;
static size_t    zero_count  = 0;

/* Precompute up to a generous limit to cover at least 10 000 terms. */
static void precompute(void)
{
    /* Bound chosen to easily cover 10 000 terms; density ≈1/3. */
    const size_t NMAX = 400000;
    const size_t SZ   = NMAX + 1;

    /* Pentagonal coefficients φ(x) = ∏ (1-x^j) */
    int8_t *phi8 = (int8_t*)calloc(SZ, sizeof(int8_t));
    phi8[0] = 1;
    for (int m = 1;; ++m) {
        size_t p1 = (size_t)m * (3ULL * m - 1) / 2;
        if (p1 > NMAX) break;
        phi8[p1] = (m % 2 == 0) ? 1 : -1;
        size_t p2 = (size_t)m * (3ULL * m + 1) / 2;
        if (p2 <= NMAX) phi8[p2] = (m % 2 == 0) ? 1 : -1;
    }

    /* Convert to int64_t for convolution */
    int64_t *phi = (int64_t*)malloc(SZ * sizeof(int64_t));
    for (size_t i = 0; i < SZ; ++i) phi[i] = (int64_t)phi8[i];
    free(phi8);

    /* φ²  and φ⁴ */
    int64_t *phi2  = (int64_t*)malloc(SZ * sizeof(int64_t));
    convolution(phi, SZ, phi, SZ, phi2, SZ);

    int64_t *phi4  = (int64_t*)malloc(SZ * sizeof(int64_t));
    convolution(phi2, SZ, phi2, SZ, phi4, SZ);

    free(phi); free(phi2);

    /* Store zero positions k≥1 */
    zero_arr = (idx_t*)malloc(SZ * sizeof(idx_t));  /* ample space */
    zero_count = 0;
    for (idx_t k = 1; k <= NMAX; ++k) {
        if (phi4[(size_t)k] == 0) {
            zero_arr[zero_count++] = k;
        }
    }
    free(phi4);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }
    /* Assumes n is within the precomputed range. */
    return (seq_t)zero_arr[n - 1];
}
