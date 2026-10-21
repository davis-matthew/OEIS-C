/*
 * A255588  Convert n to base 3, move the least significant digit to the most
 *          significant one and convert back to base 10.
 * https://oeis.org/A255588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n % 3) * 3^digits(n/3) + n/3
 * Range:     u8 n <= 243, u16 n <= 9626, u32 n <= 9626, u64 n <= 9626,
 *            u128 n <= 9626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n / 3;                /* m = n / 3 */
    seq_t m = (seq_t)temp;             /* higher digits */
    seq_t d = (seq_t)(n % 3);           /* least significant base‑3 digit */

    /* Count digits of m in base 3: L = floor_log3(m)+1 if m>0 else 0 */
    unsigned int L = 0;
    idx_t t = temp;
    while (t > 0) {
        t /= 3;
        ++L;
    }

    /* Compute 3^L */
    seq_t power = 1;
    for (unsigned int i = 0; i < L; ++i)
        power *= (seq_t)3;

    return d * power + m;
}
