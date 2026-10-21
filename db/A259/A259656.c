/*
 * A259656  Let f(x) be the absolute value of the difference between x and its
 *          base-2 reversal. a(n) is the number of times f(x) must be applied
 *          starting with n for the result to be 0.
 * https://oeis.org/A259656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of f(x)=|x - rev₂(x)| until 0,
 *            starting with n
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the number of bits needed to represent x (zero has length 0). */
static inline unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

/* Reverse the binary representation of x (no leading zeros). */
static inline seq_t rev_bits(seq_t x)
{
    if (x == 0) return 0;
    unsigned len = bitlen_seq_t(x);
    seq_t rev = 0;
    while (len--) {
        rev <<= 1;
        rev |= x & (seq_t)1;
        x >>= 1;
    }
    return rev;
}

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t count = 0;
    while (x != 0) {
        seq_t rev = rev_bits(x);
        x = (x >= rev) ? (x - rev) : (rev - x);
        ++count;
    }
    return count;
}
