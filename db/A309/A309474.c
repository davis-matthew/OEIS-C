/*
 * A309474  Digits of one of the two 3-adic integers sqrt(-1/2).
 * https://oeis.org/A309474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by 3‑adic Hensel lifting for sqrt(-1/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_uint_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_uint_cap.h"

/* Static workspace for the digits of the 3‑adic integer and its square. */
static seq_t   *digits    = NULL;   /* base‑3 digits of the 3‑adic integer */
static size_t   digits_cap = 0;
static size_t   digits_len = 0;

static unsigned int *sq      = NULL;   /* base‑3 digits of the square (after all carries) */
static size_t   sq_cap   = 0;
static size_t   sq_len   = 0;

/* Ensure that a seq_t array has at least 'needed' elements. */
static void
ensure_seq_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr   = tmp;
        *cap   = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialisation on first call. */
    if (!digits) {
        ensure_seq_cap(&digits, &digits_cap, 1);
        digits[0] = (seq_t)1;          /* a(0) = 1 */
        digits_len = 1;

        ensure_uint_cap(&sq, &sq_cap, 1);
        sq[0] = 1;                     /* square of the first digit: 1² = 1 */
        sq_len = 1;
    }

    /* Extend the precomputed digits up to index n. */
    while (digits_len <= n) {
        size_t k = digits_len;                     /* index of the new digit */

        /* Ensure capacity for digits and square arrays. */
        ensure_seq_cap(&digits, &digits_cap, k + 1);
        ensure_uint_cap(&sq, &sq_cap, 2 * k + 1);
        if (sq_len <= 2 * k) sq_len = 2 * k + 1;    /* we will touch indices up to 2k */

        /* Compute the next digit a_k using the current square. */
        unsigned int old_sq_k = (k < sq_len) ? sq[k] : 0;
        unsigned int temp = (1 + 3 - old_sq_k) % 3;   /* (1 - old_sq_k) mod 3 */
        unsigned int ak    = (2 * temp) % 3;          /* 0 ≤ a_k ≤ 2 */

        digits[k] = (seq_t)ak;

        /* Update the square: add cross terms and a_k² term. */
        for (size_t i = 0; i < k; ++i) {
            unsigned int di = (unsigned int)digits[i];
            sq[i + k] += 2 * ak * di;
        }
        sq[2 * k] += ak * ak;

        /* Propagate carries starting from position k. */
        unsigned long long carry = 0;
        for (size_t idx = k; idx < sq_len; ++idx) {
            unsigned long long val = (unsigned long long)sq[idx] + carry;
            sq[idx] = (unsigned int)(val % 3);
            carry   = val / 3;
        }
        while (carry) {
            sq[sq_len] = (unsigned int)(carry % 3);
            carry     /= 3;
            ++sq_len;
        }

        ++digits_len;
    }

    return digits[n];
}
