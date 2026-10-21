/*
 * A241404  Sum of n and the sum of the factorials of its digits.
 * https://oeis.org/A241404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (sum of factorials of the decimal digits of n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Pre‑computed factorials of the digits 0–9. */
static const seq_t fact[10] = {
    1,      /* 0! */
    1,      /* 1! */
    2,      /* 2! */
    6,      /* 3! */
    24,     /* 4! */
    120,    /* 5! */
    720,    /* 6! */
    5040,   /* 7! */
    40320,  /* 8! */
    362880  /* 9! */
};

seq_t a(idx_t n) {
    seq_t result = (seq_t) n;      /* start with n itself */
    idx_t tmp = n;                 /* copy for digit extraction */

    while (tmp > 0) {
        result += fact[tmp % 10];
        tmp /= 10;
    }
    return result;
}
