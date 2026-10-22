/*
 * A359379  a(n) = 1 if n is either a multiple of 4, or an odd multiple of 3,
 *          otherwise 0.
 * https://oeis.org/A359379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a multiple of 4, or an odd multiple of 3;
 *            otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* multiple of 4 */
    if (n % 4 == 0)
        return (seq_t)1;

    /* odd multiple of 3 */
    if (n % 3 == 0 && (n & 1))
        return (seq_t)1;

    return (seq_t)0;
}
