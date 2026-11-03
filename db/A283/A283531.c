/*
 * A283531  Number of steps to return to n through a chain-addition sequence
 *          mod 10 with window of size equal to the number of digits of n.
 * https://oeis.org/A283531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number of steps in the chain‑addition * sequence
 *            that returns to n, where each step deletes the * most
 *            significant digit of the current number, appends the * sum of
 *            the digits of the current number modulo 10 as the * new least
 *            significant digit, and the window size is the * number of...
 * Range:     u8 n <= 69, u16 n <= 9461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9461 (2026-11-01)
 * Helpers:   digit_sum, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
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
    /* Initial value and window length (in digits) */
    seq_t orig  = (seq_t)n;
    seq_t cur   = orig;
    int   len0  = num_digits(n);
    seq_t mod   = pow10((idx_t)(len0 - 1));   /* 10^(len0-1) */

    seq_t steps = 0;
    do
    {
        seq_t low = cur % mod;                         /* drop MSD */
        seq_t s   = digit_sum(cur) % (seq_t)10;         /* new LSD */
        cur = low * 10 + s;                             /* next value */
        ++steps;
    }
    while (cur != orig);

    return steps;
}
