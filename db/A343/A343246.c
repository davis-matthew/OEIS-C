/*
 * A343246  Number of partitions of n into 3 parts where at least one part
 *          divides another.
 * https://oeis.org/A343246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts where at least one
 *            part divides another
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage – the function is called a(idx_t n) */
seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < 3)
        return 0;

    for (idx_t x = 1; x <= n / 3; ++x) {
        idx_t maxY = (n - x) / 2;
        for (idx_t y = x; y <= maxY; ++y) {
            idx_t z = n - x - y;
            if (z < y)
                continue; /* ensure x ≤ y ≤ z */

            /* At least one part divides another */
            if ((y % x == 0) || (z % x == 0) || (z % y == 0))
                ++res;
        }
    }
    return res;
}
