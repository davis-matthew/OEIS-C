/*
 * A392688  Decimal expansion of the addend producing the record value of the
 *          Hardy-Littlewood constant from Jacobson (1995).
 * https://oeis.org/A392688
 *
 * Offset:    19    (a(19) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = digit at position (n-18) of the decimal expansion of
 *            3399714628553118047 (offset 19). a(n)=0 for n>37
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* decimal digits of 3399714628553118047 */
    static const seq_t digits[19] = {
        (seq_t)3, (seq_t)3, (seq_t)9, (seq_t)9, (seq_t)7, (seq_t)1,
        (seq_t)4, (seq_t)6, (seq_t)2, (seq_t)8, (seq_t)5, (seq_t)5,
        (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)8, (seq_t)0, (seq_t)4,
        (seq_t)7
    };
    if (n < 19) return (seq_t)0;
    if (n <= 37) return digits[n - 19];
    return (seq_t)0;
}
