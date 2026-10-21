/*
 * A284069  Numbers whose smallest decimal digit is 8.
 * https://oeis.org/A284069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1013, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A284069: numbers whose smallest decimal digit is 8.
   The sequence consists of all numbers that can be written using only
   the digits 8 and 9 and contain at least one 8, sorted increasingly.
   For a given index n (n ≥ 1) we find the minimal decimal length L
   such that ∑_{k=1}^{L-1} (2^k - 1) < n ≤ ∑_{k=1}^{L} (2^k - 1).
   Inside that length block the (n - previous_sum - 1)-th element is
   obtained by writing the offset in binary using exactly L bits
   and replacing 0 by 8 and 1 by 9.
*/
seq_t a(idx_t n)
{
    /* Find the minimal length L of the integer */
    idx_t cum = 0;          /* cumulative count of terms of shorter length   */
    idx_t L   = 0;          /* current length (will increase from 1 upwards) */

    for (L = 1; ; ++L) {
        idx_t block = ((idx_t)1 << L) - 1;   /* number of terms with length L  */
        if (cum + block >= n)                /* found the correct length       */
            break;
        cum += block;                        /* accumulate previous block       */
    }

    /* Offset within the block of length L (0‑based) */
    idx_t k = n - cum - 1;

    /* Convert the offset to the decimal number.
       Interpret k as a binary number of length L and
       map 0→8, 1→9.                          */
    seq_t  res = 0;
    for (idx_t i = L; i > 0; --i) {
        idx_t bit = (k >> (i - 1)) & 1;
        res = res * 10 + (bit ? 9 : 8);
    }
    return res;
}
