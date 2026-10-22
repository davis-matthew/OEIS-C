/*
 * A356442  a(n) is the least positive even number that is the unordered sum
 *          of two primes congruent mod 10 in exactly n ways.
 * https://oeis.org/A356442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 342, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A356442.
 * a(n) = least even number that is the unordered sum of two primes
 * with the same last decimal digit in exactly n ways.
 * The function builds a prime sieve and, for each residue mod 10,
 * convolves the indicator array of primes with itself via FFT.
 * Counts are accumulated for all even k up to the current sieve limit.
 * The result is searched for the desired n; if not found, the limit
 * is doubled and the computations are repeated.  The routine works
 * for any seq_t and idx_t supplied by the build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* seq_t and idx_t are defined by the build. */
static size_t global_limit = 0;          /* current maximal k processed   */
static uint8_t *prime_bool = NULL;       /* 1 = prime, 0 = composite    */
static seq_t *global_counts = NULL;      /* unordered pair counts       */

/* ------------------------------------------------------------------ */
/* Simple sieve of Eratosthenes producing a boolean array              */
static void make_prime_bool(idx_t limit)
{
    if (prime_bool) {
        free(prime_bool);
    }
    prime_bool = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!prime_bool) return;   /* caller will see 0 if allocation fails */
    if (limit < 2) return;
    for (idx_t i = 0; i <= limit; ++i) {
        prime_bool[i] = 1;
    }
    prime_bool[0] = prime_bool[1] = 0;
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (prime_bool[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                prime_bool[j] = 0;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Complex number structure for FFT if needed                        */
typedef struct { double r, i; } cplx;

/* Fast Fourier Transform (iterative, Cooley–Tukey) */
static void fft(cplx *a, size_t n, int invert)
{
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) {
            cplx tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2 * acos(-1.0) / len * (invert ? -1 : 1);
        double wlen_cos = cos(ang), wlen_sin = sin(ang);
        for (size_t i = 0; i < n; i += len) {
            double w_cos = 1.0, w_sin = 0.0;
            for (size_t j = 0; j < len / 2; ++j) {
                size_t u = i + j;
                size_t v = i + j + len / 2;
                cplx t;
                t.r = w_cos * a[v].r - w_sin * a[v].i;
                t.i = w_cos * a[v].i + w_sin * a[v].r;
                a[v].r = a[u].r - t.r;
                a[v].i = a[u].i - t.i;
                a[u].r += t.r;
                a[u].i += t.i;
                double tmp_cos = w_cos * wlen_cos - w_sin * wlen_sin;
                double tmp_sin = w_cos * wlen_sin + w_sin * wlen_cos;
                w_cos = tmp_cos;
                w_sin = tmp_sin;
            }
        }
    }
    if (invert) {
        for (size_t i = 0; i < n; ++i) {
            a[i].r /= n;
            a[i].i /= n;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Recompute all counts up to new_limit                               */
static void recompute_counts(idx_t new_limit)
{
    /* Build prime table up to new_limit */
    make_prime_bool(new_limit);

    /* Choose FFT size: next power of two > 2*new_limit */
    size_t sz = 1;
    while (sz <= 2 * new_limit) sz <<= 1;

    /* Temporary array for counts */
    seq_t *temp_counts = (seq_t *)calloc((size_t)new_limit + 1, sizeof(seq_t));
    if (!temp_counts) return; /* allocation failure */

    /* Residues corresponding to prime endings:
       1,3,7,9 for odd primes; 2 for prime 2; 5 only for prime 5
       (5 contributes only the pair 5+5=10). */
    const int residues[5] = {1, 3, 7, 9, 2};

    for (size_t r_idx = 0; r_idx < 5; ++r_idx) {
        int residue = residues[r_idx];

        /* Allocate FFT array */
        cplx *f = (cplx *)calloc(sz, sizeof(cplx));
        if (!f) {
            free(temp_counts);
            return;
        }

        /* Build base array: 1.0 where prime and residue match */
        for (idx_t i = 0; i <= new_limit; ++i) {
            if (prime_bool[i] && (i % 10 == residue)) {
                f[i].r = 1.0;
            }
        }

        /* Forward FFT */
        fft(f, sz, 0);
        /* Pointwise square (self-convolution) */
        for (size_t i = 0; i < sz; ++i) {
            double re = f[i].r;
            double im = f[i].i;
            f[i].r = re * re - im * im;
            f[i].i = 2 * re * im;
        }
        /* Inverse FFT */
        fft(f, sz, 1);

        /* Accumulate unordered counts for this residue */
        for (idx_t k = 0; k <= new_limit; k += 2) {
            long long ordered = (long long)(f[k].r + 0.5); /* round to nearest integer */
            int diag = 0;
            if ((k % 2 == 0) && prime_bool[k / 2] && ((k / 2) % 10 == residue)) {
                diag = 1;
            }
            seq_t unordered = (seq_t)((ordered - diag) / 2 + diag);
            temp_counts[k] += unordered;
        }

        free(f);
    }

    /* Handle residue 5 (prime 5 only), pair 5+5=10 contributes at most 1 */
    if (new_limit >= 10 && prime_bool[5]) {
        temp_counts[10] += 1;
    }

    /* Replace global counts array */
    if (global_counts) {
        free(global_counts);
    }
    global_counts = temp_counts;
    global_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0u) return (seq_t)2;

    /* Ensure we have a count table computed */
    if (global_limit == 0) {
        recompute_counts(200000u);            /* initial limit */
        if (!global_counts) return (seq_t)0;  /* allocation failure */
    }

    while (true) {
        /* Scan existing counts array for the desired n */
        for (idx_t k = 2; k <= global_limit; k += 2) {
            if (global_counts[k] == (seq_t)n) {
                return (seq_t)k;
            }
        }
        /* Not found yet, increase limit and recompute */
        idx_t new_limit = global_limit * 2;
        if (new_limit == global_limit) {      /* safety against overflow */
            new_limit = global_limit + 100000;
        }
        recompute_counts(new_limit);
        if (!global_counts) return (seq_t)0;   /* allocation failure */
    }
}
