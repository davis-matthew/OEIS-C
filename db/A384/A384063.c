/*
 * A384063  Partial sums of A172471.
 * https://oeis.org/A384063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m*n - floor((m-1)*(m+3)*(2m-1)/12) where m = A172471(n)
 * Range:     u8 n <= 43, u16 n <= 1704, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t g_value(seq_t m)
{
    seq_t a = m - 1;
    seq_t b = m + 3;
    seq_t c = 2 * m - 1;
    return (a * b * c) / 12;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t curr_g = g_value(1);          /* g(1) = 0 */
    seq_t m = 1;                        /* current block value */

    /* Find the block that contains index n */
    while (1) {
        seq_t next_g = g_value(m + 1);   /* g(m+1) */
        seq_t next_start = next_g - curr_g;       /* start index of block (m+1) */

        if ((seq_t)n < next_start)
            break;                     /* n lies in current block m */

        /* Advance to next block */
        ++m;
        curr_g = next_g;
    }

    /* a(n) = m*n - g(m) */
    return m * (seq_t)n - curr_g;
}
