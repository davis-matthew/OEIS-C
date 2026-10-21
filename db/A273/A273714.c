/*
 * A273714  Number of doublerises in all bargraphs having semiperimeter n
 *          (n>=2). A doublerise in a bargraph is any pair of adjacent up
 *          steps.
 * https://oeis.org/A273714
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*(n-1)*a(n-1) - 9*(n-2)*a(n-2) + 6*a(n-3) +
 *            (n-8)*a(n-4) + 2*(n-4)*a(n-5) + (n-6)*a(n-6)) / n with initial
 *            values a(2)=0, a(3)=1, a(4)=4, a(5)=14, a(6)=47, a(7)=155
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* undefined for n<2 – return 0 for safety */
    if (n < 2) return 0;

    /* base values */
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)14;
    if (n == 6) return (seq_t)47;
    if (n == 7) return (seq_t)155;

    /* sliding window for a(n-1) … a(n-6) */
    seq_t prev[6];
    prev[0] = (seq_t)155; /* a(n-1)  for n=8  */
    prev[1] = (seq_t)47;  /* a(n-2)  */
    prev[2] = (seq_t)14;  /* a(n-3)  */
    prev[3] = (seq_t)4;   /* a(n-4)  */
    prev[4] = (seq_t)1;   /* a(n-5)  */
    prev[5] = (seq_t)0;   /* a(n-6)  */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t num = 6 * (seq_t)(i - 1) * prev[0];
        num -= 9 * (seq_t)(i - 2) * prev[1];
        num += 6 * prev[2];
        num += (seq_t)(i - 8) * prev[3];
        num += 2 * (seq_t)(i - 4) * prev[4];
        num += (seq_t)(i - 6) * prev[5];
        seq_t ai = num / (seq_t)i;

        /* shift the window */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = ai;
    }

    return prev[0];
}
