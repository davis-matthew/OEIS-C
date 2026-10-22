/*
 * A376692  Numbers with the most significant digit odd and alternating parity
 *          of digits.
 * https://oeis.org/A376692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 2655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with the most significant digit odd and alternating parity of digits.
   The nth term is found by scanning successive integers and selecting those that
   satisfy the conditions. */
#include <stdbool.h>

static inline bool is_special(seq_t x)
{
    if (x <= (seq_t)0)
        return false;

    const seq_t TEN = (seq_t)10;
    const seq_t TWO = (seq_t)2;

    seq_t tmp = x;

    /* Process least‐significant digit first to get its parity. */
    seq_t digit = tmp % TEN;
    bool prevParityOdd = ((digit % TWO) != (seq_t)0);
    tmp /= TEN;

    while (tmp > (seq_t)0) {
        digit = tmp % TEN;
        bool currParityOdd = ((digit % TWO) != (seq_t)0);
        if (currParityOdd == prevParityOdd)
            return false;          /* consecutive digits same parity */
        prevParityOdd = currParityOdd;
        tmp /= TEN;
    }

    /* After processing all digits, prevParityOdd is the parity of the most
       significant digit.  It must be odd. */
    return prevParityOdd;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (1) {
        if (is_special(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
