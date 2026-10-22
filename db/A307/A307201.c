/*
 * A307201  Coordination sequence for trivalent node of type alpha in the
 *          first Moore pentagonal tiling.
 * https://oeis.org/A307201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n > 8; Initial values: a(0)=1,
 *            a(1)=3, a(2)=9, a(3)=12, a(4)=21, a(5)=30, a(6)=30, a(7)=33,
 *            a(8)=42
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small table of initial values */
    static const seq_t base[9] = {1, 3, 9, 12, 21, 30, 30, 33, 42};

    if (n <= 8)
        return base[n];

    /* ring buffer that keeps the last 9 values */
    seq_t ring[9];
    for (idx_t i = 0; i < 9; ++i)
        ring[i] = base[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t prev4 = ring[(i - 4) % 9];
        seq_t prev8 = ring[(i - 8) % 9];
        ring[i % 9] = prev4 + prev4 - prev8;  /* 2 * prev4 - prev8 */
    }

    return ring[n % 9];
}
