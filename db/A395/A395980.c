/*
 * A395980  Decimal expansion of 262144/177147 = 2^18/3^11.
 * https://oeis.org/A395980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = digit n of decimal expansion of 262144/177147
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* integral part is 1, remainder after that is 262144 % 177147 = 85097 */
    static const seq_t den = 177147;
    static const seq_t num = 262144;

    if (n == 0)
        return 0;          /* undefined for the OEIS offset, harmless */

    if (n == 1)
        return num / den;  /* integer part: 1 */

    seq_t rem = num % den; /* remainder after the integer part */
    idx_t pos;
    for (pos = 1; pos < n; ++pos) {
        rem *= 10;                 /* bring next decimal digit into place   */
        seq_t digit = rem / den;   /* the next decimal digit                */
        rem   %= den;              /* new remainder for the next step       */
        if (pos == n - 1)
            return digit;
    }
    /* unreachable for n ≥ 2, but keeps the compiler happy */
    return 0;
}
