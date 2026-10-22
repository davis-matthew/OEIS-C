/*
 * A322031  (Sum_{t=0..oo} ((-1)^t*(2*t+1)*q^((2*t+1)^2)))^3 * (Sum_{t=0..oo}
 *          q^((2*t+1)^2)) = Sum_{k=0..oo} a(k)*q^(8*k+4).
 * https://oeis.org/A322031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^(8*n+4) in (∑_{t≥0} (-1)^t (2t+1)
 *            q^((2t+1)^2))^3 * (∑_{t≥0} q^((2t+1)^2))
 * Range:     i8 n <= 5, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- internal cache ----------------------------------------------------- */
static bool          initialized = false;
static idx_t         max_n_precomputed = 0;
static seq_t        *coefs   = NULL; /* coefficients of f^3 up to current Nmax */
static seq_t        *results = NULL; /* a(k) for k ≤ max_n_precomputed */

/* ----- integer floor square root (works for idx_t up to 64 bits) ------------- */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid * mid <= n) {
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* ----- recompute cached tables up to new_max_n --------------------------- */
static void recompute(idx_t new_max_n)
{
    idx_t Nmax   = 8 * new_max_n + 4;          /* largest exponent needed   */
    idx_t sqrtN  = floor_sqrt_u64(Nmax);      /* largest odd with square ≤ Nmax */

    /* count odd numbers <= sqrtN */
    size_t odd_count = 0;
    for (idx_t v = 1; v <= sqrtN; v += 2)
        ++odd_count;

    /* allocate arrays of odd squares and their signed weights */
    int *odd_squares = (int *)calloc(odd_count, sizeof(int));
    int *odd_weights = (int *)calloc(odd_count, sizeof(int));
    size_t i = 0;
    for (int v = 1; v <= (int)sqrtN; v += 2) {
        odd_squares[i] = v * v;
        int sign = (((v - 1) / 2) & 1) ? -1 : 1;   /* (-1)^((v-1)/2) */
        odd_weights[i] = sign * v;
        ++i;
    }

    /* compute coefficients of f^3 */
    seq_t *new_coefs = (seq_t *)calloc((size_t)Nmax + 1, sizeof(seq_t));
    if (!new_coefs)
        abort();
    for (size_t a = 0; a < odd_count; ++a) {
        int w1 = odd_weights[a];
        int s1 = odd_squares[a];
        for (size_t b = 0; b < odd_count; ++b) {
            int w2 = odd_weights[b];
            int s2 = odd_squares[b];
            int w12 = w1 * w2;
            int s12 = s1 + s2;
            for (size_t c = 0; c < odd_count; ++c) {
                int w3 = odd_weights[c];
                int s3 = odd_squares[c];
                idx_t e = (idx_t)s12 + (idx_t)s3;
                if (e > Nmax)
                    continue;
                seq_t prod = (seq_t)w12 * (seq_t)w3;
                new_coefs[(size_t)e] += prod;
            }
        }
    }
    free(coefs);
    coefs = new_coefs;

    /* compute a(k) for k ≤ new_max_n */
    seq_t *new_results = (seq_t *)calloc((size_t)new_max_n + 1, sizeof(seq_t));
    if (!new_results)
        abort();
    for (idx_t n = 0; n <= new_max_n; ++n) {
        idx_t target = 8 * n + 4;
        seq_t sum = 0;
        for (size_t j = 0; j < odd_count; ++j) {
            int sq = odd_squares[j];
            if ((idx_t)sq > target)
                break;
            sum += coefs[(size_t)(target - sq)];
        }
        new_results[(size_t)n] = sum;
    }
    free(results);
    results = new_results;

    free(odd_squares);
    free(odd_weights);

    initialized = true;
    max_n_precomputed = new_max_n;
}

/* ----- public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized || n > max_n_precomputed)
        recompute(n > max_n_precomputed ? n : max_n_precomputed);
    return results[(size_t)n];
}
