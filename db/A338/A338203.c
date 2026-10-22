/*
 * A338203  Triangle read by rows: T(n,k) is the number of iterations needed
 *          to make the n-bit binary representation of k palindromic by
 *          binary-subtracting its bit reversal, 0 <= k < 2^n.
 * https://oeis.org/A338203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) is the flattened triangle of iteration counts for *
 *            binary-subtract bit‑reversal until a palindrome is reached. *
 *            For a n‑bit word k, T(n,k) is the number of iterations needed. *
 *            The index n is mapped to a row of length 2^bits, where bits = *
 *            floor(log2(n+1)). The computation proceeds by dir...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"


static inline seq_t rev_bits(seq_t x, idx_t bits)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i)
        rev = (rev << 1) | ((x >> i) & 1);
    return rev;
}

seq_t a(idx_t n)
{
    /* Determine the number of bits ('rows') for this index */
    idx_t bits = floor_log2_uint64(n + 1);

    /* Calculate the offset (k) within the row */
    idx_t start = ((idx_t)1 << bits) - 1;
    idx_t k = n - start;

    seq_t cur = (seq_t)k;
    /* Mask for modulo 2^bits arithmetic */
    seq_t mask = ((seq_t)1 << bits) - 1;

    seq_t steps = 0;
    while (1)
    {
        seq_t r = rev_bits(cur, bits);
        if (cur == r)
            break;
        cur = (cur - r) & mask;
        ++steps;
    }
    return steps;
}
