/*
 * A241929  Number of inequivalent colorings of the Fano plane with n colors.
 * https://oeis.org/A241929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+6)*(n^4+15*n^2+8)/168
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 49, u64 n <= 1175,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the output type */
    seq_t nn = (seq_t)n;

    /* Compute the three factors of the numerator */
    seq_t a1 = nn;                                   /* n */
    seq_t a2 = nn * nn + 6;                           /* n^2 + 6 */
    seq_t a3 = nn * nn * nn * nn + 15 * nn * nn + 8;  /* n^4 + 15*n^2 + 8 */

    /* Cancel the divisor 168 = 2^3 * 3 * 7 */
    static const unsigned int denom_factors[5] = {2, 2, 2, 3, 7};
    for (int i = 0; i < 5; ++i) {
        unsigned p = denom_factors[i];
        if (a1 % p == 0) {
            a1 /= p;
        } else if (a2 % p == 0) {
            a2 /= p;
        } else if (a3 % p == 0) {
            a3 /= p;
        } else {
            /* In theory this branch should never be reached because the
               final expression is an integer.  If it does occur, we fall
               back to a safe but potentially slower path: multiply all
               three terms, divide by the remaining factor, and continue. */
            seq_t prod = a1 * a2 * a3;
            prod /= p;
            /* Reset all factors to the factored product */
            a1 = prod;
            a2 = 1;
            a3 = 1;
        }
    }

    return a1 * a2 * a3;
}
