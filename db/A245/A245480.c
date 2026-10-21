/*
 * A245480  Numbers n such that the n-th cyclotomic polynomial has a root mod
 *          11.
 * https://oeis.org/A245480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A245480: Numbers n such that the n-th cyclotomic polynomial
 *  has a root mod 11.
 *
 *  Formula (recurrence):
 *      a(n) = 11 * a(n-4)   for n > 4
 *      a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 10
 *
 *  Equivalently:
 *      a(n) = d[(n-1) % 4] * 11^floor((n-1)/4)
 *  where d = {1, 2, 5, 10}.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1..4 */
    static const seq_t d[4] = { 1, 2, 5, 10 };

    if (n <= 4)
        return d[n - 1];

    /* Decompose n-1 into quotient and remainder modulo 4 */
    idx_t k = (n - 1) / 4;  /* number of times to multiply by 11   */
    idx_t j = (n - 1) % 4;  /* index into the base array d[4]      */

    seq_t value = d[j];

    /* Multiply by 11^k; loop is O(k) which is < ¼ n, sufficient for n up to 10⁴ */
    for (idx_t i = 0; i < k; ++i) {
        value *= 11;
    }

    return value;
}
