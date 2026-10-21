/*
 * A256292  Numbers which have only digits 6 and 7 in base 10.
 * https://oeis.org/A256292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th integer whose decimal digits are only 6 and 7.
 *            The sequence is obtained by considering numbers of length L =
 *            ⌊log₂(n+1)⌋. Within each length group the numbers are sorted in
 *            lexicographic order with 6 < 7. This is equivalent to
 *            interpreting the offset within the group as a binary...
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1022, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset; sequence starts at n=1 */

    /* Calculate length L = floor(log₂(n+1)) */
    idx_t temp = n + 1;
    unsigned int L = 0;
    while (temp >>= 1) { ++L; }

    /* Offset within the length‑L block */
    idx_t pow2 = (idx_t)1 << L;          /* 2^L */
    seq_t offset = (seq_t)(n + 1) - (seq_t)pow2;

    /* Build the number from most significant bit to least */
    seq_t result = 0;
    for (unsigned int i = 0; i < L; ++i) {
        /* i=0 gives the most significant bit (position L-1) */
        seq_t bit = (offset >> (L - 1 - i)) & 1;
        result = result * 10 + (bit ? 7 : 6);
    }

    return result;
}
