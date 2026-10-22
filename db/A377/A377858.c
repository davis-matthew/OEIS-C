/*
 * A377858  a(n) = Sum_{k=1..n} tan(k*Pi/(1+2*n))^4.
 * https://oeis.org/A377858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1)*(4*n*n+6*n-1)/3 Initial values: a(0)=0, a(1)=9,
 *            a(2)=90, ... (OEIS A377858)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t N = (seq_t)n;

    /* three factors of the numerator */
    seq_t t1 = N;                     /* n */
    seq_t t2 = 2 * N + 1;             /* 2n + 1 */
    seq_t t3 = 4 * N * N + 6 * N - 1; /* 4n^2 + 6n - 1 */

    /* divide one of the factors by 3 to avoid overflow */
    if (t1 % 3 == 0)
        t1 /= 3;
    else if (t2 % 3 == 0)
        t2 /= 3;
    else
        t3 /= 3;   /* guaranteed to be divisible */

    /* multiply in ascending order to keep all intermediates within bounds */
    seq_t a0 = t1, a1 = t2, a2 = t3;
    seq_t tmp;
    if (a0 > a1) { tmp = a0; a0 = a1; a1 = tmp; }
    if (a0 > a2) { tmp = a0; a0 = a2; a2 = tmp; }
    if (a1 > a2) { tmp = a1; a1 = a2; a2 = tmp; }

    seq_t prod = a0 * a1;
    prod *= a2;
    return prod;
}
