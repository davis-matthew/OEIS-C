/*
 * A264091  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 0,1 or 2,-2.
 * https://oeis.org/A264091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 16*a(n-2) - 16*a(n-3) - 41*a(n-4) * +
 *            16*a(n-5) + 16*a(n-6) - 4*a(n-7) - a(n-8) (n ≥ 9) * Initial
 *            values (1‑based): * a(1)=8, a(2)=39, a(3)=245, a(4)=1448, *
 *            a(5)=8788, a(6)=52941, a(7)=319906, a(8)=1931215
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* return one of the first eight values directly */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)39;
    if (n == 3) return (seq_t)245;
    if (n == 4) return (seq_t)1448;
    if (n == 5) return (seq_t)8788;
    if (n == 6) return (seq_t)52941;
    if (n == 7) return (seq_t)319906;
    if (n == 8) return (seq_t)1931215;

    /* buffer[0] = a(i-1), buffer[1] = a(i-2), …, buffer[7] = a(i-8) */
    seq_t buffer[8];
    buffer[0] = (seq_t)1931215; /* a(8)   */
    buffer[1] = (seq_t)319906;   /* a(7)   */
    buffer[2] = (seq_t)52941;    /* a(6)   */
    buffer[3] = (seq_t)8788;     /* a(5)   */
    buffer[4] = (seq_t)1448;     /* a(4)   */
    buffer[5] = (seq_t)245;      /* a(3)   */
    buffer[6] = (seq_t)39;       /* a(2)   */
    buffer[7] = (seq_t)8;        /* a(1)   */

    for (idx_t i = 9; i <= n; ++i)
    {
        seq_t new_val = 0;
        new_val += buffer[0] * (seq_t)4;
        new_val += buffer[1] * (seq_t)16;
        new_val -= buffer[2] * (seq_t)16;
        new_val -= buffer[3] * (seq_t)41;
        new_val += buffer[4] * (seq_t)16;
        new_val += buffer[5] * (seq_t)16;
        new_val -= buffer[6] * (seq_t)4;
        new_val -= buffer[7];

        /* shift the window: new_val becomes a(i) */
        for (int t = 7; t > 0; --t)
            buffer[t] = buffer[t - 1];
        buffer[0] = new_val;
    }

    return buffer[0];
}
