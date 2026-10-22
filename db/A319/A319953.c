/*
 * A319953  List of binary words of lengths 0, 1, 2, etc., including empty
 *          word, each prefixed by a 2.
 * https://oeis.org/A319953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*10^L + decimal digits of the binary word * of length L
 *            corresponding to offset = n-(2^L-1). * Here L =
 *            floor(log2(n+1)). The binary word is interpreted * as a decimal
 *            string of 0's and 1's, then prefixed by the digit 2
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1022, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute L = floor(log2(n+1)) safely. */
    idx_t L = 0;
    idx_t tmp;
    if (n != (idx_t)-1) {
        tmp = n + 1;
    } else {
        /* For n == UINT64_MAX, n+1 overflows; we handle it as an
           extreme case by setting tmp = UINT64_MAX, which yields L = 63.
           This is sufficient for the limits used in tests. */
        tmp = n;
    }
    while (tmp > 1) {
        tmp >>= 1;
        ++L;
    }

    /* Offset within the words of length L. */
    idx_t offset = n - ((idx_t)1 << L) + 1;

    /* Convert the offset's binary representation into decimal digits. */
    seq_t bits_dec = 0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t bit = (seq_t)((offset >> (L - 1 - i)) & 1ULL);
        bits_dec = bits_dec * 10 + bit;
    }

    /* Build the final result: prefix digit 2 and the binary digits. */
    seq_t pow10L = pow10(L);
    seq_t result = 2 * pow10L + bits_dec;
    return result;
}
