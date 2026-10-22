/*
 * A386261  a(n) = A001511(A001511(n)), where A001511 is the ruler function.
 * https://oeis.org/A386261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 + v2(1 + v2(n)), where v2(x) is the exponent of 2 in
 *            the * factorisation of x (i.e. the number of trailing zero bits
 *            of x). * This implements a(n) = A001511(A001511(n)) with
 *            A001511(n) = v2(n)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the undefined case n == 0; not required by the problem
     * but it prevents an infinite loop. */
    if (n == 0)
        return (seq_t)0;

    /* first val: v2(n) */
    idx_t v = 0;
    idx_t tmp = n;
    while ((tmp & (idx_t)1) == 0)
    {
        v++;
        tmp >>= 1;
    }

    /* B = v + 1 */
    idx_t b = v + 1;

    /* second val: v2(b) */
    idx_t v2b = 0;
    while ((b & (idx_t)1) == 0)
    {
        v2b++;
        b >>= 1;
    }

    /* return 1 + v2(b) as seq_t */
    return (seq_t)(v2b + 1);
}
