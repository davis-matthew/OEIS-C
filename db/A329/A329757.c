/*
 * A329757  Doubly octagonal pyramidal numbers.
 * https://oeis.org/A329757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1)*(n+1)*(2*n^3+n^2-n+2)*(2*n^3+n^2-n-1)/8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0u)
        return 0;

    seq_t N = (seq_t)n;
    seq_t f[5];

    /* basic factors */
    f[0] = N;
    f[1] = (seq_t)(2 * n - 1);
    f[2] = N + 1;

    /* higher‑order terms */
    seq_t t1 = N * N * N * 2;          /* 2*n^3 */
    seq_t t2 = N * N;                  /* n^2 */
    f[3] = t1 + t2 - N + 2;            /* 2*n^3 + n^2 - n + 2 */
    f[4] = t1 + t2 - N - 1;            /* 2*n^3 + n^2 - n - 1 */

    seq_t result = 1;
    int div = 8;                       /* we need to divide by 8 */

    for (int i = 0; i < 5; ++i) {
        seq_t val = f[i];

        /* divide by 2 as many times as possible while we still need to */
        while (div > 1 && (val & 1u) == 0u) {
            val >>= 1;
            div >>= 1;
        }

        result *= val;
    }

    /* any remaining division (should be 1) */
    if (div > 1)
        result /= div;

    return result;
}
