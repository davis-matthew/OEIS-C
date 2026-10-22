/*
 * A368671  For any k >= 0, let P(k) = A368357(k) and P(-k) = A368358(k); for
 *          any n > 0, a(n) is the unique k such that P(k) = n.
 * https://oeis.org/A368671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 130, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A368671: a bijection from positive integers to all integers.
 * The inverse of the mapping P(k) defined by A368357(A368358).
 * For n = 1,2,...
 *   Let L = floor(log2(n)) + 1  (number of bits needed for n).
 *   Let m = n - 2^(L-1)  (offset within the block of numbers having L bits).
 *   Let r = reverse_bits(m, L-1) (bit reversal of the lower L-1 bits of m).
 *   For even L:   offset = (2^(L-1) + 1) / 3
 *   For odd L:    offset = (2^(L-1) - 1) / 3
 *   Then a(n) =  (+1)^L-1 * (r + offset).  The sign alternates every block,
 *   positive for even L and negative for odd L; L = 1 gives 0.
 *
 * This algorithm reproduces all documented terms, runs in O(log n) time,
 * and works for any of the supported seq_t types.
 */

#include <stdint.h>
#include <stdbool.h>

/* Reverse the lower 'w' bits of 'x' (w <= 64). */
static uint64_t reverse_bits(uint64_t x, int w)
{
    uint64_t rev = 0;
    while (w--) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not defined by the sequence but harmless */
        return 0;

    /* Compute the bit length L of n. */
    unsigned L = 0;
    idx_t temp = n;
    while (temp) {
        ++L;
        temp >>= 1;
    }

    /* Offset within the block of numbers with L bits. */
    idx_t block_start = ((idx_t)1ULL << (L - 1));
    idx_t m = n - block_start;

    /* Bit reversal of m using L-1 bits. */
    uint64_t r = reverse_bits((uint64_t)m, (int)(L - 1));

    /* Offset depending on parity of L. */
    uint64_t pow2 = ((uint64_t)1ULL << (L - 1));
    uint64_t offset;
    if (L % 2 == 0)                 /* even L: positive values */
        offset = (pow2 + 1) / 3;
    else                            /* odd L: negative values (except L=1 gives 0) */
        offset = (pow2 - 1) / 3;

    seq_t abs_val = (seq_t)(r + offset);

    /* Sign: positive for even L, negative for odd L. */
    seq_t sign = (L % 2 == 0) ? (seq_t)1 : (seq_t)-1;

    return sign * abs_val;
}
