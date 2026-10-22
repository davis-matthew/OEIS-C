/*
 * A358435  Row sums of the triangular array A357498.
 * https://oeis.org/A358435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the chain A357498 row of length n. Generate the
 *            chain as: a1 = n; for i = n-1 down to 1: ai+1 = smallest integer
 *            > ai that is divisible by i Then a(n) = Σ_{k=1..n} ak / k
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = (seq_t)n;          /* current chain element */
    seq_t sum  = term / (seq_t)n;   /* a1 / n = 1 */

    for (idx_t i = n - 1; i > 0; --i) {
        seq_t div = term / (seq_t)i;   /* floor(term / i) */
        div = div + (seq_t)1;         /* next multiple */
        term = div * (seq_t)i;        /* ai+1 */
        sum = sum + term / (seq_t)i;  /* add ak / k */
    }

    return sum;
}
