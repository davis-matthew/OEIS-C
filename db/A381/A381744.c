/*
 * A381744  Expansion of exp( Sum_{k>=1} binomial(6*k-1,2*k) * x^k/k ).
 * https://oeis.org/A381744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   binom, binom_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A381744
 * Formula: a(n) = (1/n) * Σ_{k=1..n} C(6*k-1, 2*k) * a(n-k),   a(0) = 1
 * (C denotes the binomial coefficient)
 * The function memoises computed terms to allow calls in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/binom_64.h"

/* --------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;      /* a[0..] */
static idx_t  seq_cap   = 0;
static seq_t *coeff_cache = NULL;    /* coeff_cache[k] = C(6*k-1, 2*k) */
static idx_t  coeff_cap   = 0;

/* --------------------------------------------------------------------- */
static void ensure_seq_cap(idx_t n) {
    if (n + 1 <= seq_cap) return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* initialise new elements to 0 */
    for (idx_t i = seq_cap; i < new_cap; ++i) tmp[i] = 0;
    seq_cache = tmp;
    seq_cap   = new_cap;
}

static void ensure_coeff_cap(idx_t n) {
    if (n + 1 <= coeff_cap) return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(coeff_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    for (idx_t i = coeff_cap; i < new_cap; ++i) tmp[i] = 0;
    coeff_cache = tmp;
    coeff_cap   = new_cap;
}

/* Compute all terms up to n (inclusive).  Memoises in seq_cache. */
static void compute_up_to(idx_t n) {
    ensure_seq_cap(n);
    ensure_coeff_cap(n);

    /* initialise a(0)=1 once */
    if (seq_cache[0] == 0) seq_cache[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        if (seq_cache[i] != 0) continue;   /* already computed */

        /* accumulate sum with proper precision */
        if (sizeof(seq_t) <= 8) {  /* seq_t is 64‑bit or smaller */
            unsigned __int128 sum128 = 0;
            for (idx_t k = 1; k <= i; ++k) {
                /* compute or retrieve coefficient */
                if (coeff_cache[k] == 0) {
                    coeff_cache[k] = binom_64(6 * k - 1, 2 * k);
                }
                unsigned __int128 prod =
                    (unsigned __int128)coeff_cache[k] *
                    (unsigned __int128)seq_cache[i - k];
                sum128 += prod;
            }
            seq_cache[i] = (seq_t)(sum128 / (unsigned __int128)i);
        } else {                         /* arbitrary‑precision seq_t */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                if (coeff_cache[k] == 0) {
                    coeff_cache[k] = binom(6 * k - 1, 2 * k);
                }
                sum += coeff_cache[k] * seq_cache[i - k];
            }
            seq_cache[i] = sum / (seq_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    compute_up_to(n);
    return seq_cache[n];
}
