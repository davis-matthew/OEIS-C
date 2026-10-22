/*
 * A394293  Number of induced cubes in the n X n white bishop graph.
 * https://oeis.org/A394293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)*(3*(((-1)^n) - 1) + n*(n + 1)*(n + 2))/24
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1, result for 0 is irrelevant */

    /* Use signed 128‑bit integer for intermediate calculations to avoid overflow */
    __int128 nn   = n;                     /* n as signed 128‑bit */
    __int128 sign = (nn & 1) ? -1 : 1;     /* (-1)^n */
    __int128 delta = 3 * (sign - 1) +
                     nn * (nn + 1) * (nn + 2);
    __int128 numerator = (nn + 1) * delta;
    __int128 res = numerator / 24;         /* divisible by 24 */
    return (seq_t)res;
}
