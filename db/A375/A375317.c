/*
 * A375317  Expansion of (1 + x)^2/(1 - x^2*(1 + x)^3).
 * https://oeis.org/A375317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + 3*a(n-4) + a(n-5) Initial values:
 *            a(0)=1, a(1)=2, a(2)=2, a(3)=5, a(4)=11
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0u) return 1;
    if (n == 1u || n == 2u) return 2;
    if (n == 3u) return 5;
    if (n == 4u) return 11;

    /* Circular buffer for the last five values:
       v[0] = a(i-5), v[1] = a(i-4), v[2] = a(i-3),
       v[3] = a(i-2), v[4] = a(i-1)  */
    seq_t v[5] = {1, 2, 2, 5, 11};

    for (idx_t i = 5u; i <= n; ++i) {
        seq_t next = v[3] + 3 * v[2] + 3 * v[1] + v[0];
        /* shift the window one step forward */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return v[4];
}
