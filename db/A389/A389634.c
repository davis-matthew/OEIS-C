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
 * Helpers:   meets_abundance, sigma_of_k
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
#include "../utils/meets_abundance.h"
#include "../utils/sigma_of_k.h"

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
    seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
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
