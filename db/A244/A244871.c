/*
 * A244871  Number of magic labelings with magic sum n of 3rd graph shown in
 *          link.
 * https://oeis.org/A244871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6/48 + 3*n^5/16 + 77*n^4/96 + 2*n^3 + 37*n^2/12 -
 *            n*(-1)^n/32 + 89*n/32 - 3*(-1)^n/64 + 67/64
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate results.  */
    __int128 nn = n;          /* promote the index */
    __int128 n2 = nn * nn;    /* n^2 */
    __int128 n3 = n2 * nn;    /* n^3 */
    __int128 n4 = n3 * nn;    /* n^4 */
    __int128 n5 = n4 * nn;    /* n^5 */
    __int128 n6 = n5 * nn;    /* n^6 */

    /* (-1)^n :  1 if n even, -1 if n odd */
    int sign = (n & 1) ? -1 : 1;

    __int128 sum = 0;
    sum += 4   * n6;             /*  n^6   * 4          (48 -> 4) */
    sum += 36  * n5;             /*  n^5   * 36         (16 -> 12*3) */
    sum += 154 * n4;             /*  n^4   * 154        (96 -> 2*77) */
    sum += 384 * n3;             /*  n^3   * 384        (1 -> 384) */
    sum += 592 * n2;             /*  n^2   * 592        (12 -> 16*37) */
    sum -= 6   * nn * sign;      /* -6 n (-1)^n           */
    sum += 534 * nn;             /* 534 n                  (89*6) */
    sum -= 9   * sign;           /* -9 (-1)^n              (3*3) */
    sum += 201;                  /* +201                    (67*3) */

    /* All denominators have lcm 192; perform exact division. */
    __int128 res = sum / 192;

    return (seq_t)res;
}
