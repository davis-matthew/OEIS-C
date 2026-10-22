/*
 * A390052  a(1) = 1. For n > 1, a(n) is the smallest number obtainable by
 *          rearranging the digits of n whose value has the largest prime
 *          factor among all such rearrangements.
 * https://oeis.org/A390052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A390052 – permutation with maximal largest prime factor.
 *
 *  For n > 1, a(n) is the smallest number obtainable by rearranging the
 *  digits of n whose value has the largest prime factor among all such
 *  rearrangements.  Leading zeros are dropped.
 *
 *  The implementation enumerates all permutations of the decimal digits of
 *  n, computes their largest prime factor (using the provided
 *  `largest_prime_factor()` utility), and selects the permutation with
 *  the greatest such factor, breaking ties by the smallest value.
 *
 *  No hard‑coded values are used; the sequence can be calculated for any
 *  n ≥ 1.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/largest_prime_factor.h"   /* provides uint64_t largest_prime_factor(uint64_t) */

/* Generate the next lexicographic permutation of an array of uint8_t.
 * Returns false if there is no next permutation (i.e. the array is
 * in descending order). */
static bool next_permutation(uint8_t *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    uint8_t tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) { tmp = a[l]; a[l] = a[r]; a[r] = tmp; ++l; --r; }
    return true;
}

/* Helper that performs the real computation and stores the result in *res. */
static void compute_a_impl(idx_t n_input, seq_t *res)
{
    /* Handle the trivial case. */
    if (n_input == (idx_t)1) {
        *res = (seq_t)1;
        return;
    }

    /* Extract decimal digits of n into an array. */
    uint8_t digits[20];
    int d = 0;
    uint64_t tmp = (uint64_t)n_input;
    if (tmp == 0) {                     /* not expected for n >= 1 */
        digits[d++] = 0;
    } else {
        while (tmp > 0) {
            digits[d++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to most‑significant‑first order. */
        for (int i = 0; i < d / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[d - 1 - i];
            digits[d - 1 - i] = t;
        }
    }

    /* Sort ascending to start from the lexicographically smallest permutation. */
    for (int i = 0; i < d - 1; ++i)
        for (int j = i + 1; j < d; ++j)
            if (digits[i] > digits[j]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }

    uint64_t best_val = 0;
    uint64_t best_lpf = 0;
    uint8_t perm[20];
    for (int i = 0; i < d; ++i) perm[i] = digits[i];

    do {
        uint64_t val = 0;
        for (int i = 0; i < d; ++i) val = val * 10 + perm[i];

        uint64_t lpf = (val <= 1) ? 1 : largest_prime_factor(val);

        if (lpf > best_lpf || (lpf == best_lpf && val < best_val)) {
            best_lpf = lpf;
            best_val = val;
        }
    } while (next_permutation(perm, d));

    *res = (seq_t)best_val;
}

/* Public API satisfying the required signature. */
seq_t a(idx_t n)
{
    seq_t result;
    compute_a_impl(n, &result);
    return result;
}
