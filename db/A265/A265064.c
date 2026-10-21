/*
 * A265064  Coordination sequence for (2,5,5) tiling of hyperbolic plane.
 * https://oeis.org/A265064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) – a(n-6) (n ≥ 7) Initial values:
 *            a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=13, a(5)=19, a(6)=26
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 61, u64 n <= 127,
 *            u128 n <= 259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First seven terms */
    static const seq_t init[7] = {1, 3, 5, 8, 13, 19, 26};

    if (n < 7)
        return init[n];

    /* Ring buffer of the last seven terms */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = buf[(i - 2) % 7] + buf[(i - 3) % 7] +
                    buf[(i - 4) % 7] - buf[(i - 6) % 7];
        buf[i % 7] = val;
    }

    return buf[n % 7];
}
