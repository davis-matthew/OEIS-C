/*
 * A309932  Distance from n to closest palindrome that is different from n.
 * https://oeis.org/A309932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = distance from n to the nearest distinct palindrome
 * Range:     u8 n <= 574, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digits, pow10, update_best
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/update_best.h"

/* Reverse the decimal digits of x. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x = x / 10;
    }
    return rev;
}

/* Construct the palindrome of total_digits digits having the given
   first_half (the left half including the middle digit when odd). */
static seq_t make_pal(seq_t first_half, idx_t total_digits)
{
    idx_t half = (total_digits + 1) / 2;
    seq_t pal;
    if (total_digits % 2 == 0) {
        /* even number of digits: mirror entire first_half */
        seq_t rev = reverse_digits(first_half);
        seq_t shift = pow10(half);
        pal = first_half * shift + rev;
    } else {
        /* odd number of digits: drop the middle digit before mirroring */
        seq_t rev = reverse_digits(first_half / 10);
        seq_t shift = pow10(half - 1);
        pal = first_half * shift + rev;
    }
    return pal;
}

/* Sequence a(n) for OEIS A309932. */
seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    int d = digits(n);                 /* number of decimal digits of n */
    idx_t half = (d + 1) / 2;
    seq_t shift_pow = pow10(d - half);
    seq_t first_half = n_val / shift_pow;

    seq_t best = 0;
    bool has_best = false;

    /* Same‑length palindrome from n itself. */
    seq_t pal_same = make_pal(first_half, d);
    update_best(pal_same, n_val, &has_best, &best);

    /* Lower first_half (only if it still has the same digit length). */
    if (first_half > 1) {
        seq_t pal_lower = make_pal(first_half - 1, d);
        update_best(pal_lower, n_val, &has_best, &best);
    }

    /* Upper first_half (keep same digit length). */
    seq_t max_first_half = pow10(half) - 1;
    if (first_half < max_first_half) {
        seq_t pal_upper = make_pal(first_half + 1, d);
        update_best(pal_upper, n_val, &has_best, &best);
    }

    /* Largest palindrome with one less digit (all 9's). */
    if (d > 1) {
        seq_t lower_len = pow10(d - 1) - 1;
        update_best(lower_len, n_val, &has_best, &best);
    }

    /* Smallest palindrome with one more digit (10^d + 1). */
    seq_t upper_len = pow10(d) + 1;
    update_best(upper_len, n_val, &has_best, &best);

    return best;
}
