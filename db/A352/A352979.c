/*
 * A352979  a(n) = Sum_{k=1..n} Sum_{j=1..k} Sum_{i=1..j} (k*j*i)^3.
 * https://oeis.org/A352979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n+1)^2 * (n+2) * (n+3) * * (35*n^6 + 205*n^5 +
 *            263*n^4 - 221*n^3 - * 214*n^2 + 324*n - 112)/13440
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 is handled explicitly to avoid
       negative intermediate values with unsigned types.
       The formula yields zero when n = 0 anyway. */
    if (n == 0)
        return (seq_t)0;

    seq_t N  = (seq_t)n;            /* n               */
    seq_t N2 = N * N;               /* n^2             */
    seq_t N3 = N2 * N;              /* n^3             */
    seq_t N4 = N3 * N;              /* n^4             */
    seq_t N5 = N4 * N;              /* n^5             */
    seq_t N6 = N5 * N;              /* n^6             */

    /* Polynomial part: 35*n^6 + 205*n^5 + 263*n^4
       - 221*n^3 - 214*n^2 + 324*n - 112   */
    seq_t poly = (seq_t)35 * N6
               + (seq_t)205 * N5
               + (seq_t)263 * N4
               - (seq_t)221 * N3
               - (seq_t)214 * N2
               + (seq_t)324 * N
               - (seq_t)112;

    /* Assemble the final product:
        a(n) = poly * n^2 * (n+1)^2 * (n+2) * (n+3) / 13440
    */
    seq_t result = poly;
    result *= N2;                  /* * n^2 */
    result *= (N + 1);             /* * (n+1) */
    result *= (N + 1);             /* * (n+1) again */
    result *= (N + 2);             /* * (n+2) */
    result *= (N + 3);             /* * (n+3) */
    result /= (seq_t)13440;        /* exact division */

    return result;
}
