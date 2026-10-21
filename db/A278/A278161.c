/*
 * A278161  Run length transform of A008619 (floor(n/2)+1).
 * https://oeis.org/A278161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{runs r of 1-bits in binary(n)} (⌊len(r)/2⌋ + 1) Here
 *            a(0) = 1 by convention (empty set of runs)
 * Range:     u8 n <= 9654, u16 n <= 9654, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t product = 1;           /* accumulate the result */
    idx_t run_len;               /* length of the current run of 1's */

    /* Process every bit of n */
    while (n != 0) {
        if ((n & 1) == 0) {      /* skip zero bits */
            n >>= 1;
            continue;
        }
        /* Count contiguous 1's */
        run_len = 0;
        while ((n & 1) != 0) {
            ++run_len;
            n >>= 1;
        }
        /* Compute A008619(run_len) = floor(run_len / 2) + 1 */
        product *= ((seq_t)run_len / 2 + 1);
    }
    return product;
}
