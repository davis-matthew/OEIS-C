/*
 * A295041  The Grundy number of restricted Nim with a pass move.
 * https://oeis.org/A295041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Grundy numbers for restricted Nim with a pass
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned r = (unsigned)n & 7u;   /* n mod 8 */

    if (r == 1) {
        /* a(8k+1) = 2k+1 = ((n-1)/8)*2 + 1 */
        return ((seq_t)(n - 1u) / 8u) * 2 + 1;
    }
    if (r == 5) {
        /* a(8k+5) = 2k = ((n-5)/8)*2 */
        return ((seq_t)(n - 5u) / 8u) * 2;
    }
    if (r == 0 || r == 4) {
        /* a(4k)   = 2k+1 = n/2 + 1   (k>0) */
        return ((seq_t)n / 2u) + 1;
    }
    if (r == 2 || r == 6) {
        /* a(4k+2) = 2k   = n/2 - 1   */
        return ((seq_t)n / 2u) - 1;
    }
    /* r == 3 or 7: a(4k+3) = a(floor(n/2)) */
    return a(n / 2u);
}
