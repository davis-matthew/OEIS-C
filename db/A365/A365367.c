/*
 * A365367  Number of steps for iteration of map x -> (5/3)*round(x) to reach
 *          an integer > n when started at n, or -1 if no such integer is ever
 *          reached.
 * https://oeis.org/A365367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 1 such that iterating x_{k+1} =
 *            (5/3)·round(x_k) starting from x_0 = n yields an integer > n.
 *            The iteration is implemented by keeping y_k = round(x_k) and
 *            computing y_{k+1} = round(5·y_k/3)
 * Range:     u8 n <= 4, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)-1;                /* undefined for n = 0 */

    seq_t curr = (seq_t)n;               /* y_0 = round(x_0) = n */
    seq_t step = (seq_t)0;

    while (1) {
        ++step;                          /* apply one iteration */

        seq_t numer = (seq_t)5 * curr;    /* 5·y_k */

        if ((numer % (seq_t)3) == (seq_t)0) {   /* y_k*5 is divisible by 3 */
            seq_t val = numer / (seq_t)3;       /* integer x_{k+1} */
            if (val > (seq_t)n)                 /* first integer > n? */
                return step;
        }

        /* compute next rounded value: round(5·y_k/3) = (5·y_k + 1)/3 */
        curr = (numer + (seq_t)1) / (seq_t)3;
    }

    /* unreachable, but keeps the compiler happy */
    return (seq_t)-1;
}
