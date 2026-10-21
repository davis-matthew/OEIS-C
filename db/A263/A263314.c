/*
 * A263314  Numbers m whose decimal representation includes at least one digit
 *          that divides every digit of m.
 * https://oeis.org/A263314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate positive integers and return the nth
   number whose decimal digits contain a digit that divides every
   digit of the number. */

#include <stdbool.h>

static bool has_dividing_digit(seq_t m)
{
    /* store digits of m in reverse order */
    unsigned char digits[64];
    idx_t len = 0;

    if (m == 0) return false;               /* should not happen */

    seq_t tmp = m;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp = tmp / 10;
    }

    /* try each non‑zero digit as a potential divisor */
    for (idx_t i = 0; i < len; ++i) {
        unsigned char d = digits[i];
        if (d == 0) continue;               /* zero cannot divide anything */

        bool ok = true;
        for (idx_t j = 0; j < len; ++j) {
            if (digits[j] % d != 0) {       /* not divisible */
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;

    while (true) {
        if (has_dividing_digit(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + 1;
    }
}
