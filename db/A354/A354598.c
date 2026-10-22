/*
 * A354598  Maximal GCD of eight positive integers with sum n.
 * https://oeis.org/A354598
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor d of n with d <= n/8 (maximal GCD of 8
 *            positive integers summing to n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)             /* below offset – value irrelevant */
        return 0;

    idx_t limit = n / 8;   /* integer floor of n/8 */
    idx_t d = 1;           /* 1 always divides n */
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0)
            d = i;      /* i is a divisor, and increasing i ensures the last one is the maximum */
    }
    return (seq_t)d;
}
