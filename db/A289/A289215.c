/*
 * A289215  a(n) = n! * Laguerre(n,-9).
 * https://oeis.org/A289215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{i=0..n} 9^i / i! * C(n,i)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t j = 1; j <= n; ++j)
        fact *= (seq_t)j;

    /* initial term for i = 0 : n! */
    seq_t term = fact;
    seq_t sum  = term;

    /* iterate i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = term * ((seq_t)9 * (seq_t)(n - (i - 1)));
        seq_t denom = (seq_t)i * (seq_t)i;   /* i^2, division is exact */
        seq_t next = mult / denom;

        sum += next;
        term = next;
    }
    return sum;
}
