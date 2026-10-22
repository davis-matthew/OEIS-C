/*
 * A365459  a(n) = n - the largest power of 3 that is less than or equal to n.
 * https://oeis.org/A365459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - the largest power of 3 that is <= n
 * Range:     u8 n <= 359, u16 n <= 9301, u32 n <= 9301, u64 n <= 9301,
 *            u128 n <= 9301, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t power = 1;

    /* Handle n == 0 gracefully (though offset starts at 1). */
    if (n == 0)
        return 0;

    /* Find the largest power of 3 not exceeding n. */
    while (power <= nn / 3) {
        power *= 3;
    }

    return nn - power;
}
