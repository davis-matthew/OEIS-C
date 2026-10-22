/*
 * A319289  The x coordinates of the shell enumeration of N X N where N = {0,
 *          1, 2, ...} (A319514).
 * https://oeis.org/A319289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * Let N = floor(sqrt(n)) (the shell index). * Let k = n -
 *            N*N (position inside that shell, 0‑based). * If N = 0, return 0.
 *            * If N is odd: * if k <= N return k; * else return N; * If N is
 *            even: * if k <= N return N; * else return (N-1) - (k-(N+1)); *
 *            This returns the x‑coordinate of the nth...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_idx(idx_t n)
{
    /* compute floor(sqrt(n)) for 0 <= n < 2^64 */
    idx_t x = 0;
    for (int i = 31; i >= 0; --i) {
        idx_t y = x | (1ULL << i);
        if (y * y <= n)
            x = y;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t N = isqrt_idx(n);          /* floor(sqrt(n)) */
    idx_t k = n - N * N;             /* offset within shell N */
    idx_t x;

    if (N == 0)
        x = 0;
    else if (N & 1ULL)               /* N is odd */
    {
        if (k <= N)
            x = k;
        else
            x = N;
    }
    else /* N is even */
    {
        if (k <= N)
            x = N;
        else
            x = (N - 1ULL) - (k - (N + 1ULL));
    }

    return (seq_t)x;
}
