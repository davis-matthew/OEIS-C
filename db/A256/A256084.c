/*
 * A256084  Non-palindromic balanced numbers in base 4.
 * https://oeis.org/A256084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non‑palindromic balanced integer in base 4
 * Range:     u8 n <= 5, u16 n <= 1947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number is balanced if ∑(digit_i · (i − (L−1)/2)) = 0, where i is the
 * position from the left (0 = most‑significant) and L is the number of digits.
 * The equivalent integer test is ∑(digit_i · (2·i_left − (L−1))) = 0,
 * with i_left = L−1−i. The number must not read the same forwards and
 * backwards. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: test whether a decimal number is balanced in base 4
 * and non‑palindromic.  The argument is an unsigned integer type
 * that can hold the value.  The function returns true only if
 * both conditions are satisfied.  It works for any size of
 * the argument type as long as it can store the candidate value. */
static bool balanced_nonpal(idx_t value)
{
    /* Extract base‑4 digits into an array (least significant first). */
    idx_t tmp = value;
    idx_t digits[64];          /* 64 digits is enough for any 128‑bit value. */
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % 4;
        tmp /= 4;
    }
    if (len == 0)             /* value 0 has no digits; treat as non‑balanced */
        return false;

    /* Weighted sum test: Σ digit_i * (2*i_left − (len−1)) == 0. */
    int64_t sum2 = 0;          /* signed to handle negative contributions. */
    for (size_t i = 0; i < len; ++i) {
        int64_t digit   = (int64_t)digits[i];
        int64_t i_left  = (int64_t)(len - 1 - i);
        int64_t weight  = 2 * i_left - (int64_t)(len - 1);
        sum2 += digit * weight;
    }
    if (sum2 != 0)
        return false;          /* not balanced */

    /* Non‑palindromic test. */
    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i])
            return true;       /* first mismatch -> non‑palindromic */
    }
    return false;              /* all digits matched -> palindrome */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;           /* number of balanced non‑palindromic numbers found */
    idx_t candidate = 0;       /* start checking from 1 */

    for (;;) {
        ++candidate;
        if (!balanced_nonpal(candidate))
            continue;
        ++count;
        if (count == n)
            return (seq_t)candidate;
    }
}
