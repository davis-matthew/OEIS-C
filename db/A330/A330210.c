/*
 * A330210  Numbers that can be expressed as the sum of 2 prime numbers in a
 *          prime number of different ways.
 * https://oeis.org/A330210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   small_prime_test
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that can be expressed as the sum of 2 prime numbers in a
 * prime number of different ways (OEIS A330210).
 *
 * The function a(n) returns the nth such number, 1‑based.
 *
 * Implementation: For a given upper bound N we use a
 * fast convolution (FFT) to count, for every even m ≤ N, the number of
 * ordered Goldbach partitions.  From that we obtain the number of
 * unordered partitions, then we test whether that number is prime.
 * All qualifying even numbers are stored in a static cache, which is
 * enlarged as needed until it contains at least n terms.
 *
 * The code satisfies the required API: `seq_t a(idx_t n)`.  It uses
 * only integer arithmetic for the sequence values and standard C/C++
 * constructs. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/small_prime_test.h"

/* --------- FFT helper (complex numbers) ---------- */
typedef struct { double re, im; } Complex;

static void fft(Complex *a, size_t n, bool invert)
{
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            Complex tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    const double PI = acos(-1.0);
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        Complex wlen = { cos(ang), sin(ang) };
        for (size_t i = 0; i < n; i += len) {
            Complex w = { 1.0, 0.0 };
            for (size_t j = 0; j < len / 2; ++j) {
                Complex u = a[i + j];
                /* multiply a[i+len/2] by w */
                double v_re = a[i + j + len / 2].re * w.re -
                              a[i + j + len / 2].im * w.im;
                double v_im = a[i + j + len / 2].re * w.im +
                              a[i + j + len / 2].im * w.re;
                a[i + j].re = u.re + v_re;
                a[i + j].im = u.im + v_im;
                a[i + j + len / 2].re = u.re - v_re;
                a[i + j + len / 2].im = u.im - v_im;

                double w_re = w.re * wlen.re - w.im * wlen.im;
                double w_im = w.re * wlen.im + w.im * wlen.re;
                w.re = w_re;
                w.im = w_im;
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            a[i].re /= n;
            a[i].im /= n;
        }
    }
}

/* --------- cache of qualifying terms ---------- */
static seq_t *cache_terms   = NULL;   /* array of terms */
static size_t cache_len     = 0;      /* number stored */

static idx_t current_limit = 0;       /* maximum m that has been processed */

/* --------- recompute terms up to 'limit' ---------- */
static void recompute(idx_t limit)
{
    if (limit <= current_limit) return;   /* nothing new to do */

    /* --------- sieve of Eratosthenes up to 'limit' ---------- */
    size_t sz = (size_t)limit + 1;
    bool *prime_mask = (bool*)calloc(sz, sizeof(bool));
    if (!prime_mask) return;       /* out of memory */

    for (size_t i = 0; i < sz; ++i) prime_mask[i] = true;
    if (sz > 0) prime_mask[0] = false;
    if (sz > 1) prime_mask[1] = false;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (prime_mask[i])
            for (idx_t j = i * i; j <= limit; j += i)
                prime_mask[j] = false;

    /* --------- build convolution array for Goldbach counts ---------- */
    size_t N = 1;
    while (N <= (size_t)(2 * limit)) N <<= 1;
    Complex *A = (Complex*)calloc(N, sizeof(Complex));
    if (!A) { free(prime_mask); return; }

    for (idx_t i = 2; i <= limit; ++i)
        if (prime_mask[i]) A[i].re = 1.0;   /* A[i] = 1 if i is prime */

    fft(A, N, false);                     /* forward FFT */

    /* element‑wise square -> convolution of prime indicator with itself */
    for (size_t i = 0; i < N; ++i) {
        double re = A[i].re;
        double im = A[i].im;
        A[i].re = re * re - im * im;
        A[i].im = 2 * re * im;
    }

    fft(A, N, true);                      /* inverse FFT */

    /* --------- build cache of qualifying even numbers ---------- */
    seq_t *new_cache = NULL;
    size_t new_cap   = 0;
    size_t new_len   = 0;

    for (idx_t m = 4; m <= limit; m += 2) {
        long long ordered = (long long)(A[m].re + 0.5);   /* round to nearest */
        bool half_prime = prime_mask[m / 2];
        long long unordered = (ordered + (half_prime ? 1 : 0)) / 2;
        if (small_prime_test((idx_t)unordered)) {
            if (new_len == new_cap) {
                new_cap = new_cap ? new_cap * 2 : 64;
                new_cache = (seq_t*)realloc(new_cache, new_cap * sizeof(seq_t));
            }
            new_cache[new_len++] = (seq_t)m;
        }
    }

    /* replace old cache */
    if (cache_terms) free(cache_terms);
    cache_terms = new_cache;
    cache_len   = new_len;
    current_limit = limit;

    free(prime_mask);
    free(A);
}

/* --------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety for undefined index */

    /* increase limit until we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 2000;
        recompute(new_limit);
    }

    return cache_terms[(size_t)(n - 1)];
}
