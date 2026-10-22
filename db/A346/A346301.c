/*
 * A346301  Positions of words in A076478 such that first digit = last digit.
 * https://oeis.org/A346301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the nth binary word (from A076478) whose
 *            first digit equals the last digit. The n-th qualifying word has
 *            length L where 2^(L-1) < n ≤ 2^L (with the convention L=1 for
 *            n=1,2). Its numeric value v among all binary words of length L
 *            is: - if n ≤ 2^(L-1): v = 2 * (n - 1) (first dig...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the length L of the word containing the nth qualifying word. */
    idx_t L = 0;
    idx_t pow2 = 1;
    while (pow2 < n) {
        pow2 <<= 1;   /* pow2 = 2^L after loop */
        L++;
    }
    if (n == 1)      /* special case for n=1 -> L must be 1 */
        L = 1;

    /* Compute the offset of the nth qualifying word within words of length L. */
    seq_t offset;
    if (L == 1) {
        offset = (seq_t)(n - 1);   /* 0 or 1 for the two words of length 1 */
    } else {
        offset = (seq_t)(n - ((idx_t)1 << (L - 1)) - 1);
    }

    /* Numeric value v of the qualifying word in ascending order of all words of length L. */
    seq_t v;
    if (L == 1) {
        v = offset;   /* v = 0 or 1 */
    } else {
        idx_t half = (idx_t)1 << (L - 2);   /* 2^(L-2)  (number of words with first bit 0) */
        if (offset < (seq_t)half) {
            v = (seq_t)offset * 2;          /* first bit 0: v = 2 * offset */
        } else {
            idx_t m = (idx_t)(offset - (seq_t)half);
            v = ((seq_t)1 << (L - 1)) + 1;  /* 2^(L-1) + 1  (first bit 1) */
            v += (seq_t)m * 2;              /* then add 2*m (middle bits) */
        }
    }

    /* Overall position in A076478: a(n) = 2^L - 1 + v. */
    seq_t start = ((seq_t)1 << L) - 1;
    return start + v;
}
