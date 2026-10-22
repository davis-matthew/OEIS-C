/*
 * A333912  Numbers k such that phi(k) is not the sum of 3 squares, where phi
 *          is the Euler totient function (A000010).
 * https://oeis.org/A333912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 8118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333912: numbers k such that φ(k) is NOT a sum of three squares. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/calc_phi.h"  /* declares seq_t calc_phi(seq_t m); */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> terms_vec;   /* cache of the sequence terms */
#else
static seq_t *terms_arr = NULL;        /* cache of the sequence terms (C) */
static size_t terms_len = 0;           /* number of terms stored (C) */
static size_t terms_cap = 0;           /* current capacity (C) */
#endif

static uint64_t last_k = 0;            /* last integer examined */

/* Test whether a number can be expressed as a sum of three integer squares
   (Legendre's theorem). */
static bool is_sum_of_three_squares(seq_t m)
{
    seq_t t = m;
    while ((t & 3) == 0) t >>= 2;          /* divide by 4 until not divisible by 4 */
    return (t & 7) != 7;                   /* t mod 8 for the Legendre condition */
}

#ifndef __cplusplus
/* Ensure the C array has capacity for at least 'needed' elements. */
static void ensure_capacity_c(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(terms_arr, new_cap * sizeof(seq_t));
    if (!new_arr) {                       /* allocation failed; terminate */
        exit(1);
    }
    terms_arr = new_arr;
    terms_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for out‑of‑range index */

#ifdef __cplusplus
    /* C++ build: use std::vector to manage storage. */
    while (terms_vec.size() < (size_t)n) {
        ++last_k;
        seq_t phi_val = calc_phi((seq_t)last_k);
        if (!is_sum_of_three_squares(phi_val)) {
            terms_vec.push_back((seq_t)last_k);
        }
    }
    return terms_vec[(size_t)n - 1];
#else
    /* C build: use a manually managed dynamic array. */
    while (terms_len < (size_t)n) {
        ++last_k;
        seq_t phi_val = calc_phi((seq_t)last_k);
        if (!is_sum_of_three_squares(phi_val)) {
            ensure_capacity_c(terms_len + 1);
            terms_arr[terms_len++] = (seq_t)last_k;
        }
    }
    return terms_arr[(size_t)n - 1];
#endif
}
