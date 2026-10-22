/*
 * A375820  Decimal expansion of 333/106.
 * https://oeis.org/A375820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of decimal expansion of 333/106 at position n
 *            (1-indexed)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Integer part: 333 / 106 = 3 remainder 15 */
    if (n == 1) return 3;

    unsigned long long rem = 333 % 106;  /* start with remainder after integer part */
    seq_t digit = 0;

    /* Compute decimal digits one by one */
    for (idx_t i = 1; i < n; ++i) {
        unsigned long long tmp = rem * 10ULL;      /* shift remainder left by one decimal place */
        digit = (seq_t)(tmp / 106ULL);             /* next decimal digit */
        rem   = tmp % 106ULL;                      /* new remainder for next iteration */
    }
    return digit;
}
