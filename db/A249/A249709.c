/*
 * A249709  Number of length 3+3 0..n arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 44*n)/15 + 2*n^4 + 7*n^3 + 7*n^2 + 1 Recurrence:
 *            a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (n > 6) Initial values: a(1)=20, a(2)=125, a(3)=440,
 *            a(4)=1153, a(5)=2524, a(6)=4893
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 139, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t base[] = {20, 125, 440, 1153, 2524, 4893};

    if (n <= 6)
        return base[n - 1];

    /* buffer: buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[5] = a(k-6) */
    seq_t buf[6] = { base[5], base[4], base[3], base[2], base[1], base[0] };

    for (idx_t k = 7; k <= n; ++k)
    {
        /* Use 128‑bit intermediate to avoid overflow on 64‑bit seq_t */
        __int128 t =
              6 * (__int128)buf[0]
            - 15 * (__int128)buf[1]
            + 20 * (__int128)buf[2]
            - 15 * (__int128)buf[3]
            + 6 * (__int128)buf[4]
            -   (__int128)buf[5];

        seq_t next = (seq_t)t;

        /* Shift buffer: discard oldest, insert newest at the front */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];
}
