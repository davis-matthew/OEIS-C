/*
 * A255072  Number of steps to reach 0 starting with n and using the iterated
 *          process: x -> x - (number of runs in binary representation of x).
 * https://oeis.org/A255072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 0 via x -> x - (number of runs
 *            in binary representation of x)
 * Range:     u8 n <= 1259, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

static inline idx_t run_count(idx_t n)
{
    /* number of runs in the binary representation of n */
    return (n == 0) ? 0 : popcount_idx(n ^ (n >> 1));
}

seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n)
    {
        idx_t runs = run_count(n);
        n -= runs;          /* n >= runs for n > 0 */
        ++steps;
    }
    return steps;
}
