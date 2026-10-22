/*
 * A333470  Lexicographically earliest sequence of distinct positive terms
 *          such that a(n) is the number of commas that a(n) has to step over
 *          (to the right) to be met by an odd term. This odd term might not
 *          be the closest odd term to a(n).
 * https://oeis.org/A333470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n if floor(log2(n)) is even; otherwise a(n) = n+1 for
 *            even n, n-1 for odd n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(log2(n)) */
    idx_t k = 0;
    idx_t m = n;
    while (m >>= 1) {
        ++k;
    }

    if ((k & 1) == 0) {
        return (seq_t)n;          /* n is in A053738 */
    } else {
        /* swap with the adjacent term */
        if ((n & 1) == 0) {
            return (seq_t)n + 1;  /* even n → opposite odd */
        } else {
            return (seq_t)n - 1;  /* odd n  → opposite even */
        }
    }
}
