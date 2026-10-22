/*
 * A369841  n-th digit of the decimal expansion of 1/n, with the digit to the
 *          left of the decimal point counted as the first digit.
 * https://oeis.org/A369841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10^(n-1) / n) mod 10
 * Range:     u8 n <= 25, u16 n <= 6910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 just in case (though the sequence is defined for n ≥ 1). */
    if (n == 0)
        return (seq_t)0;

    /* m = n, m10 = 10*n */
    seq_t m = (seq_t)n;
    seq_t m10 = m * (seq_t)10;

    /* Compute r = 10^(n-1) mod n and t = 10^(n-1) mod (10*n) */
    seq_t r = (seq_t)1 % m;
    seq_t t = (seq_t)1 % m10;

    for (idx_t i = 1; i < n; ++i) {
        r = (r * (seq_t)10) % m;
        t = (t * (seq_t)10) % m10;
    }

    /* (t - r) is divisible by n, and (t - r) / n = (10^(n-1) / n) mod 10 */
    seq_t qmod10 = (t - r) / m;
    return (seq_t)(qmod10 % (seq_t)10);
}
