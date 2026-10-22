/*
 * A363228  Exponent of 4 in 9^n - 1.
 * https://oeis.org/A363228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = floor((3 + v_2(n))/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v_2(n) is the exponent of 2 in n (number of trailing zeros in binary) */

seq_t a(idx_t n)
{
    /* compute v_2(n) */
    idx_t v2 = 0;
    while ((n & 1) == 0) {
        v2++;
        n >>= 1;
    }
    /* floor division by 2 is automatic for unsigned integers */
    return (seq_t)((3 + v2) / 2);
}
