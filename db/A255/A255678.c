/*
 * A255678  Smallest number m needing exactly n distinct digits in decimal
 *          representation of m*(m+1)/2.
 * https://oeis.org/A255678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest m such that T(m)=m*(m+1)/2 has exactly n
 *            distinct decimal digits
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned count_distinct_digits_idx(idx_t x)
{
    unsigned used[10] = {0};
    if (x == 0) return 1;
    while (x > 0) {
        unsigned d = x % 10;
        used[d] = 1;
        x /= 10;
    }
    unsigned cnt = 0;
    for (int i = 0; i < 10; ++i)
        if (used[i]) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* No integer value can have more than ten distinct decimal digits. */
    if (n < 1 || n > 10) return (seq_t)0;

    for (idx_t m = 0; ; ++m) {
        idx_t tri = m * (m + 1) / 2;
        if (count_distinct_digits_idx(tri) == (unsigned)n) {
            return (seq_t)m;
        }
    }
}
