/*
 * A271499  Positive numbers n such that the number of 1's in the binary
 *          expansion of n is not a power of 2.
 * https://oeis.org/A271499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: positive integers whose binary popcount is NOT a power of 2. */
#include "../utils/is_power_of_two.h"

/* Efficient popcount for any seq_t type.  Returns the number of set bits as an idx_t. */
static idx_t popcount_seq(seq_t v)
{
    idx_t cnt = 0;
    seq_t x = v;
    while (x) {
        /* Remove the lowest set bit.  The expression 'x &= x - 1' is valid for all
           integer types supporting the bitwise operators. */
        x &= x - (seq_t)1;
        ++cnt;
    }
    return cnt;
}

/* Return the nth term of A271499 (offset 1).  For n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but harmless */

    seq_t num = 1;
    idx_t found = 0;

    while (1) {
        idx_t ones = popcount_seq(num);
        if (!is_power_of_two((seq_t)ones)) {
            ++found;
            if (found == n)
                return num;
        }
        ++num;   /* advance to the next candidate */
    }
}
