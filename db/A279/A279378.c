/*
 * A279378  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) + 3*a(n-4) + a(n-5) Initial
 *            values: a(1)=1, a(2)=3, a(3)=8, a(4)=22, a(5)=61
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = {1, 3, 8, 22, 61};
    if (n <= 5)
        return init[n - 1];

    seq_t v[5] = {init[0], init[1], init[2], init[3], init[4]};
    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        next = (seq_t)2 * v[4] + v[3] + (seq_t)2 * v[2] + (seq_t)3 * v[1] + v[0];
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }
    return v[4];
}
