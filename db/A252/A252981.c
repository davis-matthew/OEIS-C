/*
 * A252981  Number of nX6 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A252981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 10, a(n) = 63438848*n^2 - 1019729920*n + 4176308004. For
 *            n ≤ 10 the exact values are used
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exact values for n = 1 … 10 */
    static const seq_t small[11] = {
        0,          /* dummy for index 0 */
        10,         /* a(1) */
        85,         /* a(2) */
        851,        /* a(3) */
        8487,       /* a(4) */
        67537,      /* a(5) */
        432842,     /* a(6) */
        5672484,    /* a(7) */
        36112108,   /* a(8) */
        129234988,  /* a(9) */
        322183180   /* a(10) */
    };

    if (n <= 10)
        return small[n];

    /* Compute the quadratic formula for n > 10.
       All intermediate results fit in signed 64‑bit for n ≤ 10 000. */
    unsigned long long ns = (unsigned long long)n;
    unsigned long long part1 = 63438848ULL * ns * ns;     /* 63438848 * n^2 */
    unsigned long long part2 = 1019729920ULL * ns;        /* 1019729920 * n */
    long long diff = (long long)part1 - (long long)part2 + 4176308004LL;
    return (seq_t)diff;
}
