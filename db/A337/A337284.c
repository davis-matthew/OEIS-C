/*
 * A337284  a(n) = Sum_{i=1..n} (i-1)*T(i)^2, where T(i) = A000073(i) is the
 *          i-th tribonacci number.
 * https://oeis.org/A337284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (i-1) * T(i)^2, where T(1)=0, T(2)=1, T(3)=1
 *            are the Tribonacci numbers (T(k)=T(k-1)+T(k-2)+T(k-3))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage, should be compiled together with the build's definitions of seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=0.  For n<1 (should not happen) return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* Tribonacci initial values: T(1)=0, T(2)=1, T(3)=1 */
    seq_t t1 = 0;   /* T(1) */
    seq_t t2 = 1;   /* T(2) */
    seq_t t3 = 1;   /* T(3) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t Ti;
        if (i == 1) {
            Ti = t1;
        } else if (i == 2) {
            Ti = t2;
        } else if (i == 3) {
            Ti = t3;
        } else {
            Ti = t1 + t2 + t3;   /* next Tribonacci number */
            t1 = t2;             /* shift the window */
            t2 = t3;
            t3 = Ti;
        }

        /* Add (i-1)*Ti^2 to the running sum.  For i==1 the term is zero. */
        if (i > 1) {
            seq_t sq = Ti * Ti;
            seq_t term = ((seq_t)(i - 1)) * sq;
            sum += term;
        }
    }

    return sum;
}
