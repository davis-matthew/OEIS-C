/*
 * A374362  a(n) is the least term t of A005836 such that n - t also belongs
 *          to A005836.
 * https://oeis.org/A374362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by writing n in base 3, replacing each 1 by 0
 *            and each 2 by 1, and interpreting the resulting digits as a
 *            base‑3 number. Equivalently, a(n) = Σ 3^i for every i where the
 *            i‑th ternary digit of n equals 2
 * Range:     u8 n <= 511, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* the computed a(n) */
    seq_t pow3   = 1;   /* current power of 3 (3^i) */

    while (n > 0)
    {
        idx_t digit = n % 3;   /* lowest ternary digit */
        if (digit == 2)
            result += pow3;    /* add 3^i when digit is 2 */

        pow3 *= 3;              /* move to next ternary place */
        n /= 3;
    }

    return result;
}
