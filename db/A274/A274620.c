/*
 * A274620  If n^2 has an even number of digits, write n after the left half
 *          of the digits of n^2 and before the right half, otherwise if n^2
 *          has 2t+1 digits, write n after the first t digits of n^2 and
 *          before the last t+1 digits.
 * https://oeis.org/A274620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat( floor(n² / 10^ceil(d/2)), n, n² mod 10^ceil(d/2)
 *            ), where d = digits(n²)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9295 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;                /* n² */
    int d = num_digits(nsq);                         /* number of digits of n² */

    int right_len;
    if (d % 2 == 0) {
        right_len = d / 2;                          /* even: left_len = right_len */
    } else {
        right_len = d / 2 + 1;                      /* odd: ceil(d/2) */
    }

    seq_t pow_right = pow10((idx_t)right_len);      /* 10^right_len */
    seq_t left_part = nsq / pow_right;              /* left half of n² */
    seq_t right_part = nsq % pow_right;             /* right half of n² */

    int dig_n = num_digits((seq_t)n);                /* digits of n */
    seq_t pow_left = pow10((idx_t)(dig_n + right_len)); /* 10^(digits(n)+right_len) */

    return left_part * pow_left + (seq_t)n * pow_right + right_part;
}
