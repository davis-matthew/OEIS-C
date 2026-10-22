/*
 * A321804  Keep only consecutive identical decimal digits of n; write -1 if
 *          all digits disappear.
 * https://oeis.org/A321804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 199, i16 n <= 199, i32 n <= 199, i64 n <= 199,
 *            i128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Keep only consecutive identical decimal digits of n;
   write -1 if all digits disappear.
   Example: 123321 → 33, 1123 → 11, 100223 → 22, 12321 → -1. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n (least significant first) */
    idx_t temp = n;
    uint8_t digits[64];   /* enough for 64‑bit integers */
    int dcnt = 0;
    if (temp == 0) {
        digits[dcnt++] = 0;
    } else {
        while (temp > 0) {
            digits[dcnt++] = (uint8_t)(temp % 10);
            temp /= 10;
        }
    }

    /* Process runs of consecutive identical digits (most significant first) */
    int i = dcnt - 1;
    int run_digit = digits[i];
    int run_len = 1;
    seq_t result = 0;
    bool had_run = false;
    --i;
    for (; i >= 0; --i) {
        if (digits[i] == run_digit) {
            run_len++;
        } else {
            if (run_len >= 2) {
                /* value of the run: dddd… (run_len times) */
                seq_t run_val = 0;
                seq_t pow10 = 1;
                for (int k = 0; k < run_len; ++k) {
                    run_val = run_val * 10 + run_digit;
                    pow10 *= 10;
                }
                result = result * pow10 + run_val;
                had_run = true;
            }
            run_digit = digits[i];
            run_len = 1;
        }
    }
    /* last run */
    if (run_len >= 2) {
        seq_t run_val = 0;
        seq_t pow10 = 1;
        for (int k = 0; k < run_len; ++k) {
            run_val = run_val * 10 + run_digit;
            pow10 *= 10;
        }
        result = result * pow10 + run_val;
        had_run = true;
    }

    return had_run ? result : (seq_t)-1;
}
