/*
 * A389634  Numbers k whose abundancy (sigma(k)/k) is at least 4/3, where
 *          sigma(k) = A000203(k).
 * https://oeis.org/A389634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 4455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9450 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389634: Numbers k such that σ(k)/k ≥ 4/3.
   The routine hunts down the n‑th such integer by generating k
   from 2 upwards, computing σ(k) via elementary prime
   factorisation, and checking the inequality σ(k) ≥ (4/3)k.
   Results are memoised so subsequent calls are fast. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: compute σ(k) for a 64‑bit integer k. This uses a simple
   trial‑division factorisation that is fast enough for the sizes
   required by the tests. */
static seq_t sigma_of_k(idx_t k) {
    idx_t n = k;
    seq_t sum = 1;               /* product of (p^(e+1)-1)/(p-1) for each prime factor */
    idx_t d = 2;
    while ((seq_t)d * (seq_t)d <= (seq_t)n) {
        if (n % d == 0) {
            seq_t term = 1;      /* 1 + d + d^2 + ... + d^e */
            seq_t power = 1;
            while (n % d == 0) {
                n /= d;
                power *= d;
                term += power;
            }
            sum *= term;
        }
        d = (d == 2 ? 3 : d + 2);   /* 2, then odd numbers only */
    }
    if (n > 1) {
        /* remaining prime factor exponent = 1 */
        sum *= ((seq_t)n + 1);
    }
    return sum;
}

/* Helper: test whether σ(k)/k ≥ 4/3 */
static bool meets_abundance(idx_t k) {
    seq_t sigma = sigma_of_k(k);
    seq_t left  = sigma * (seq_t)3;
    seq_t right = ((seq_t)k) * (seq_t)4;
    return left >= right;
}

/* static cache of computed sequence values */
static seq_t *values = NULL;
static size_t values_len = 0;
static size_t values_cap = 0;

/* last integer examined; 0 means nothing examined yet. */
static idx_t last_checked = 1;

/* --------------------------------------------------------------------- */

/* Ensure the values array has room for 'needed' elements. */
static void ensure_capacity_seq(size_t needed) {
    if (needed <= values_cap) return;
    size_t new_cap = values_cap ? values_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, values, new_cap);
    if (!tmp) abort();  /* allocation failed */
    values = tmp;
    values_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Public interface: return the n-th element of A389634 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* behaviour below offset is irrelevant */

    while (values_len < n) {
        /* examine next integer */
        last_checked++;
        if (meets_abundance(last_checked)) {
            ensure_capacity_seq(values_len + 1);
            values[values_len++] = last_checked;
        }
    }
    return values[n - 1];
}
