/*
 * A339255  Leading digit of n in base 5.
 * https://oeis.org/A339255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( n / 5^floor(log_5(n)) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit of n in base 5, with offset 1 (a(1)=1) */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;           /* offset guard; not used for n>=1 */
    idx_t pow = 1u;
    /* Find the largest power of 5 that does not exceed n.
       Use division to avoid overflow on multiplication. */
    while (pow <= n / 5u) {
        pow *= 5u;
    }
    return (seq_t)(n / pow);
}
