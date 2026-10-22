/*
 * A346524  Write n in ternary, replace each 2 with -1, interpret each "-" as
 *          a subtraction operator, and evaluate the resulting expression in
 *          ternary.
 * https://oeis.org/A346524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A346524.
   Algorithm steps:
   1. Convert n into ternary digits (most‑significant first).
   2. Building an expression string by replacing each digit 2 with the
      two characters '-' and '1'.
   3. Evaluate that string as a ternary expression where each '-' is
      a subtraction operator (a leading '-' is unary).
   The function runs in O(number of ternary digits) time. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Step 1: extract ternary digits (least‑significant first). */
    uint64_t tmp = n;
    int d[64];
    int len = 0;
    do {
        d[len++] = tmp % 3;
        tmp /= 3;
    } while (tmp);
    /* Reverse to most‑significant first. */
    for (int i = 0; i < len / 2; ++i) {
        int t = d[i];
        d[i] = d[len - 1 - i];
        d[len - 1 - i] = t;
    }

    /* Step 2: build the expression in a temporary buffer. */
    /* Each digit contributes at most two characters. */
    char expr[128];
    int e = 0;
    for (int i = 0; i < len; ++i) {
        int dig = d[i];
        if (dig == 2) {
            expr[e++] = '-';
            expr[e++] = '1';
        } else {
            expr[e++] = '0' + dig; /* 0 or 1 */
        }
    }

    /* Step 3: evaluate the expression. */
    seq_t result = 0;
    seq_t num = 0;
    int sign = 1;
    int p = 0;

    /* Handle possible leading unary minus. */
    if (p < e && expr[p] == '-') {
        sign = -1;
        ++p;
    }

    while (p < e) {
        /* Accumulate the next number. */
        while (p < e && expr[p] != '-') {
            /* digits are only '0' or '1' */
            num = num * 3 + (seq_t)(expr[p] - '0');
            ++p;
        }
        result += sign * num;
        num = 0;
        /* If a minus operator follows, the next number is subtracted. */
        if (p < e && expr[p] == '-') {
            sign = -1;
            ++p;
        } else {
            /* No further operators; end of expression. */
            sign = 1;
        }
    }

    return result;
}
