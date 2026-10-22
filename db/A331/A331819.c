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
 * Helpers:   negabinary_digit_sum_of_negk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A331819 by brute‑force search: iterate k = 1,2,…,
   test if -k is a negabinary‑Niven number (k divisible by the sum
   of digits of its negabinary representation).  Returns the nth term
   (n ≥ 1). */

#include <stdint.h>
#include "../utils/negabinary_digit_sum_of_negk.h"

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
