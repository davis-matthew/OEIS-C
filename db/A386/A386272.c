/*
 * A386272  Expansion of 1/(1 - 49*x)^(3/7).
 * https://oeis.org/A386272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(7*n-4)/n * a(n-1), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the multiplicative recurrence derived from the D-finite equation
   n*a(n) + 7*(-7*n+4)*a(n-1) = 0.  Computing from n=1 up to the requested
   index yields the desired term exactly for any integer type seq_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)7 * ((seq_t)7 * i - (seq_t)4);
        res = (res * num) / (seq_t)i;
    }
    return res;
}
