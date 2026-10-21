/*
 * A298474  a(n) is the least k such that A090701(k) = n.
 * https://oeis.org/A298474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n-4 for n even; a(n) = 3*n-3 for odd n ≠ 1,5 (special
 *            cases: a(1)=1, a(5)=11)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special small cases */
    if (n == 1) return (seq_t)1;
    if (n == 5) return (seq_t)11;

    /* compute using simple arithmetic */
    seq_t x = (seq_t)n;
    if ((n & 1) == 0)           /* n even */
        return x * (seq_t)3 - (seq_t)4;
    else                         /* n odd (except 1,5 handled above) */
        return x * (seq_t)3 - (seq_t)3;
}
