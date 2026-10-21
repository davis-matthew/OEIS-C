/*
 * A259705  Row sums of A259704.
 * https://oeis.org/A259705
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((n-4)/2 + 1) * (n-1)! / 3, with a(2)=0, a(3)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)   /* a(2)=0, a(3)=0, and the function is only defined for n≥2 */
        return 0;

    /* factorial(n-1) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* divide by 3 (exact for n≥4 because (n-1)! contains a factor 3) */
    fact /= 3;

    /* numerator ((n-4)/2 + 1) */
    seq_t numer = (seq_t)((n - 4) / 2 + 1);

    return fact * numer;
}
