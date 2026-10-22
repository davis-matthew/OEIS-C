/*
 * A352230  Numbers that can be expressed as the sum of two primes in exactly
 *          8 ways.
 * https://oeis.org/A352230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352230: Numbers that can be expressed as the sum of two primes in exactly 8 ways (Goldbach partitions). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- complex number structure for FFT ---------- */
typedef struct {
    double re;
    double im;
} complex_t;

/* ---------- iterative Cooley–Tukey FFT ---------- */
static void fft(complex_t *a, size_t n, int invert)
{
    /* bit reversal */
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            complex_t tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    /* iterative FFT */
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * acos(-1.0) / len * (invert ? -1 : 1);
        complex_t wlen = { cos(ang), sin(ang) };
        for (size_t i = 0; i < n; i += len) {
            complex_t w = { 1.0, 0.0 };
            for (size_t j = 0; j < len / 2; ++j) {
                complex_t u = a[i + j];
                complex_t v = {
                    w.re * a[i + j + len / 2].re - w.im * a[i + j + len / 2].im,
                    w.re * a[i + j + len / 2].im + w.im * a[i + j + len / 2].re
                };
                a[i + j].re = u.re + v.re;
                a[i + j].im = u.im + v.im;
                a[i + j + len / 2].re = u.re - v.re;
                a[i + j + len / 2].im = u.im - v.im;

                double tmp_re = w.re * wlen.re - w.im * wlen.im;
                double tmp_im = w.re * wlen.im + w.im * wlen.re;
                w.re = tmp_re;
                w.im = tmp_im;
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

/* ---------- cached sequence (uint64_t) ---------- */
static uint64_t *seq_arr      = NULL;
static size_t seq_cap        = 0;
static size_t seq_len        = 0;

static void seq_append(uint64_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_arr = (uint64_t *)realloc(seq_arr, new_cap * sizeof(uint64_t));
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static const precomputation for all calls */
    enum { LIMIT = 2000000 };                 /* upper bound for sieve and convolutions */
    static bool initialized = false;
    static uint8_t *is_prime = NULL;          /* 0/1 flag array */
    static uint32_t *primes   = NULL;         /* list of primes up to LIMIT */
    static size_t primes_cnt  = 0;

    if (!initialized) {
        /* sieve of Eratosthenes up to LIMIT */
        size_t size = LIMIT + 1;
        is_prime = (uint8_t *)malloc(size * sizeof(uint8_t));
        for (size_t i = 0; i < size; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }

        /* collect primes into array */
        primes_cnt = 0;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) primes_cnt++;

        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        size_t k = 0;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) primes[k++] = (uint32_t)i;

        /* convolution via FFT: self-convolution of prime indicator */
        size_t conv_size = 1;
        while (conv_size < 2 * LIMIT + 1) conv_size <<= 1;

        complex_t *fa = (complex_t *)calloc(conv_size, sizeof(complex_t));
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            fa[p].re = 1.0;
        }

        fft(fa, conv_size, 0);                   /* forward transform */
        for (size_t i = 0; i < conv_size; ++i) {  /* square each element */
            double re = fa[i].re;
            double im = fa[i].im;
            fa[i].re = re * re - im * im;
            fa[i].im = 2.0 * re * im;
        }
        fft(fa, conv_size, 1);                   /* inverse transform */

        /* collect even numbers with exactly 8 unordered prime partitions */
        for (size_t sum = 4; sum <= LIMIT; sum += 2) {
            long long conv = (long long)(fa[sum].re + 0.5); /* round to nearest integer */
            long long same_prime = 0;
            if (sum % 2 == 0 && is_prime[sum / 2])
                same_prime = 1;
            long long unordered = (conv + same_prime) / 2;
            if (unordered == 8) {
                seq_append((uint64_t)sum);
            }
        }

        free(fa);
        free(primes);
        /* is_prime can be kept for future calls */
        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;
    return (seq_t)(seq_arr[n - 1]);
}
