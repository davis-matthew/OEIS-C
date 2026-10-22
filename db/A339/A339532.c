/*
 * A339532  Numbers b > 1 such that the smallest three primes, i.e., 2, 3 and
 *          5 are base-b Wieferich primes.
 * https://oeis.org/A339532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base + ⌊(n-1)/8⌋ * 900 + prefix[(n-1) mod 8] where base =
 *            449 and prefix = {0,108,144,200,252,308,344,452,900} This
 *            follows from the recurrence a(n)=a(n-1)+a(n-8)-a(n-9) with
 *            a(1)=449
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)          /* not defined for n<1, but this case is not expected */
        return 0;
    if (n == 1)         /* a(1) = 449 */
        return 449;

    static const seq_t prefix[9] = {0,108,144,200,252,308,344,452,900};
    const seq_t base = 449;
    idx_t m = n - 1;            /* number of differences to sum */
    idx_t q = m / 8;            /* full periods of 8 differences */
    idx_t r = m % 8;            /* remaining differences */
    seq_t result = base + (seq_t)q * 900; /* base + repetitions of full period */
    result += prefix[r];               /* add the remaining partial period */
    return result;
}
