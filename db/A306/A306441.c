/*
 * A306441  Next larger integer with same number of runs of 1's in its binary
 *          representation as n.
 * https://oeis.org/A306441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Next larger integer with same number of runs of 1's in binary representation as n */

/* Count the number of set bits of a seq_t value */
static unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) {
        c++;
        x &= x - 1;   /* drop lowest set bit */
    }
    return c;
}

/* Number of runs of 1's in the binary representation of x */
static unsigned run_count(seq_t x)
{
    /* a run starts when a bit is 1 and the previous bit is 0 */
    return popcount_seq(x & (~(x >> 1)));
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    seq_t cur = (seq_t)n;       /* current value to compare */
    unsigned r = run_count(cur);/* desired run count */

    seq_t m = cur + 1;          /* candidate for the next integer */
    while (run_count(m) != r) {
        m++;                    /* try the next larger integer */
    }
    return m;
}
