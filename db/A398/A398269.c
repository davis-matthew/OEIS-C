/*
 * A398269  Number of ways to partition the standard Roman numeral
 *          representation of n into a sequence of one or more substrings,
 *          each of which is itself the standard Roman numeral representation
 *          of a positive integer.
 * https://oeis.org/A398269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2^max(m-1,0) * Π g(d_i) where g(0..9) =
 *            {1,1,2,4,2,1,2,4,8,2} and d3,d2,d1,d0 are the
 *            thousands,hundreds,tens,units digits of n
 * Range:     u8 n <= 187, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t g[10] = {1, 1, 2, 4, 2, 1, 2, 4, 8, 2};

    /* Extract decimal digits of n (last four). */
    idx_t d0 = n % 10;
    idx_t d1 = (n / 10) % 10;
    idx_t d2 = (n / 100) % 10;
    idx_t d3 = (n / 1000) % 10;

    /* Compute product of the g-values. */
    seq_t prod = g[(unsigned)d0] * g[(unsigned)d1];
    prod *= g[(unsigned)d2] * g[(unsigned)d3];

    /* Count non‑zero digits. */
    unsigned m = 0;
    if (d0) m++;
    if (d1) m++;
    if (d2) m++;
    if (d3) m++;

    /* Multiply by 2^(m-1) if m≥1. */
    if (m > 0) {
        unsigned exp = m - 1;
        prod <<= exp;          /* equivalent to prod *= 1 << exp */
    }

    return prod;
}
