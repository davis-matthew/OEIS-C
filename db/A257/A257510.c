/*
 * A257510  Number of nonleading zeros in factorial base representation of n
 *          (A007623).
 * https://oeis.org/A257510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zero digits in the factorial base
 *            representation of n
 * Range:     u8 n <= 9007, u16 n <= 9007, u32 n <= 9007, u64 n <= 9007,
 *            u128 n <= 9007, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t k = 1;
    idx_t t = n;
    while (t > 0) {
        if ((t % (k + 1)) == 0)
            count++;
        t /= (k + 1);
        k++;
    }
    return count;
}
