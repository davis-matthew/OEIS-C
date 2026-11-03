/*
 * A265565  Smallest base-5 palindrome m >= n, written in base 5.
 * https://oeis.org/A265565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-5 palindrome m ≥ n, returned as the decimal
 *            value obtained by interpreting m’s base‑5 digits as a decimal
 *            number
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Convert a number to its base‑5 digit representation and
   interpret those digits as a decimal number (e.g. 6 → base5 “11” → 11). */
static seq_t base5_as_decimal(seq_t x) {
    int digits[128];
    int len = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        digits[len++] = (int)(t % (seq_t)5);
        t /= (seq_t)5;
    }
    /* Zero case: return 0. */
    if (len == 0) {
        return (seq_t)0;
    }
    seq_t result = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}

/* Determine whether x written in base‑5 is a palindrome */
static bool is_palindrome_base5(seq_t x) {
    /* Base‑5 digits are stored least significant first. */
    int digits[128];
    int len = 0;

    seq_t t = x;
    while (t > (seq_t)0) {
        digits[len++] = (int)(t % (seq_t)5);
        t /= (seq_t)5;
    }
    /* For x == 0 the digit array is empty; 0 is a palindrome. */
    for (int i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i]) {
            return false;
        }
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    while (!is_palindrome_base5(m)) {
        m++;
    }
    return base5_as_decimal(m);
}
