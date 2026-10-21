/*
 * A253944  a(n) = 3*binomial(n+1,7).
 * https://oeis.org/A253944
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n+1,7)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 6 the sequence is not defined in OEIS; return 0. */
    if (n < 6)
        return (seq_t)0;

    const idx_t k = 7;          /* binomial coefficient k = 7         */
    seq_t res = (seq_t)1;

    /* Compute C(n+1,7) multiplicatively */
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n + 1 - (i - 1))) / (seq_t)i;
    }

    /* a(n) = 3 * C(n+1,7) */
    res = res * (seq_t)3;
    return res;
}
