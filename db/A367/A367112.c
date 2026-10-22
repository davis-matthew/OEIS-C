/*
 * A367112  G.f. satisfies A(x) = 1 + 2*x*A(x) + x^3*A(x)^3.
 * https://oeis.org/A367112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(n,3*k) *
 *            A001764(k) A001764(k) = binomial(3*k, k) / (2*k + 1) The
 *            function memoises intermediate results to support arbitrary call
 *            order
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

static seq_t *pow2_vals = NULL;   /* pow2_vals[i] = 2^i */
static size_t pow2_cap = 0;

static seq_t *c_vals = NULL;      /* c_vals[k] = A001764(k) */
static size_t c_cap = 0;

static seq_t *a_vals = NULL;      /* a_vals[n] = a(n) */
static size_t a_cap = 0;
static size_t a_len = 0;          /* how many a-values are currently stored */

/* Ensure pow2_vals has entries up to n (inclusive) */
static void ensure_pow2(idx_t n) {
    size_t new_cap = (size_t)n + 1;
    if (pow2_cap < new_cap) {
        seq_t *tmp = (seq_t*)realloc(pow2_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2_vals = tmp;
        size_t start = pow2_cap;
        if (start == 0) {
            pow2_vals[0] = (seq_t)1;
            start = 1;
        }
        for (size_t i = start; i < new_cap; ++i)
            pow2_vals[i] = pow2_vals[i - 1] * (seq_t)2;
        pow2_cap = new_cap;
    }
}

/* Ensure c_vals has entries up to needed_k (inclusive) */
static void ensure_c(idx_t needed_k) {
    size_t new_cap = (size_t)needed_k + 1;
    if (c_cap < new_cap) {
        seq_t *tmp = (seq_t*)realloc(c_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        c_vals = tmp;
        size_t start = c_cap;
        for (size_t k = start; k < new_cap; ++k) {
            if (k == 0) {
                c_vals[0] = (seq_t)1;
            } else {
                seq_t num = binomial((idx_t)(3 * k), (idx_t)k);
                seq_t den = (seq_t)(2 * k + 1);
                c_vals[k] = num / den;
            }
        }
        c_cap = new_cap;
    }
}

/* Compute a(n) for all indices up to n, storing them in a_vals. */
static void ensure_a(idx_t n) {
    if (a_len <= (size_t)n) {
        ensure_pow2(n);
        ensure_c(n / 3);

        size_t new_cap = (size_t)n + 1;
        if (a_cap < new_cap) {
            seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            a_vals = tmp;
            a_cap = new_cap;
        }

        for (idx_t i = a_len; i <= n; ++i) {
            idx_t kmax = i / 3;
            seq_t sum = (seq_t)0;
            seq_t comb = (seq_t)1;      /* binomial(i,0) */
            idx_t m = 0;                /* current m for comb */

            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t term = pow2_vals[i - 3 * k] * comb * c_vals[k];
                sum += term;

                if (k < kmax) {
                    /* Advance comb to binomial(i, m+3) */
                    for (int step = 1; step <= 3; ++step) {
                        seq_t num = (seq_t)(i - (m + step - 1));
                        seq_t den = (seq_t)(m + step);
                        comb = comb * num / den;   /* exact division */
                    }
                    m += 3;
                }
            }
            a_vals[i] = sum;
        }
        a_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n) {
    ensure_a(n);
    return a_vals[n];
}
