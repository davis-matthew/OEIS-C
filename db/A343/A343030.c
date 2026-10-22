/*
 * A343030  Number of 1-bits in the binary expansion of n which have an odd
 *          number of 0-bits at less significant bit positions.
 * https://oeis.org/A343030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 1-bits in the binary expansion of n whose *
 *            number of 0-bits at less significant positions is odd
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t   res   = 0;   /* answer */
    idx_t   zeros = 0;   /* count of zeros seen so far (lower positions) */
    idx_t   t     = n;   /* working copy */

    while (t) {                /* iterate over bits from LSB to MSB */
        if (t & 1ULL) {        /* current bit is 1 */
            if (zeros & 1)     /* odd number of lower zeros? */
                res++;          /* count this 1-bit */
        } else {
            zeros++;           /* current bit is 0 */
        }
        t >>= 1ULL;           /* move to next bit */
    }
    return res;
}
