/*
 * A260586  Concatenated sums of pairs of adjacent digits in the concatenation
 *          of the numbers from 1 to n.
 * https://oeis.org/A260586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = concatenation of sums of each pair of adjacent digits in the decimal string "123…n". */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1, there are no adjacent pairs. */
    if (n < 2) {
        return 0;
    }

    seq_t result = 0;
    /* Pre‑compute 10ⁱ for i = 1, 2 to convert the sum into a digit string. */
    const seq_t pow10_1 = pow10((idx_t)1);
    const seq_t pow10_2 = pow10((idx_t)2);

    /* Store most significant digit of previous number to form adjacent pairs. */
    int prev_digit = -1;     /* -1 indicates no previous digit yet. */

    /* Buffer for the digits of the current integer (max 20 decimal digits for uint64_t). */
    int digits[21];
    for (idx_t num = 1; num <= n; ++num) {
        /* Decompose num into digits (least significant first). */
        idx_t tmp = num;
        int dcount = 0;
        do {
            digits[dcount++] = (int)(tmp % 10);
            tmp /= 10;
        } while (tmp > 0);

        /* Process digits from most to least significant. */
        for (int k = dcount - 1; k >= 0; --k) {
            int cur = digits[k];
            if (prev_digit != -1) {
                int sum = prev_digit + cur;
                /* Append the decimal representation of the sum to result. */
                seq_t mul = (sum < 10) ? pow10_1 : pow10_2;
                result = result * mul + (seq_t)sum;
            }
            prev_digit = cur;
        }
    }

    return result;
}
