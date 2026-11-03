/*
 * A331819  Positive numbers k such that -k is a negative negabinary-Niven
 *          number, i.e., divisible by the sum of digits of its negabinary
 *          representation (A027615).
 * https://oeis.org/A331819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9632 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A331819 by brute‑force search: iterate k = 1,2,…,
   test if -k is a negabinary‑Niven number (k divisible by the sum
   of digits of its negabinary representation).  Returns the nth term
   (n ≥ 1). */

#include <stdint.h>

static inline int negabinary_digit_sum_of_negk(seq_t k)
{
    /* m is the signed value of -k; k will be small for the required
       range, so int64_t is safe. */
    int64_t m = -(int64_t)k;
    int sum = 0;
    while (m != 0) {
        int r = m & 1;          /* least significant bit (0 or 1) */
        sum += r;
        /* Next value: m = (m - r) / (-2)  (exact division) */
        m = (m - r) / -2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    while (1) {
        int sum = negabinary_digit_sum_of_negk(k);
        if (sum > 0 && (k % (seq_t)sum) == 0) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
