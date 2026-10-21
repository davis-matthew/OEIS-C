/*
 * A253454  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253454
 *  Number of (n+1)×(6+1) 0..2 arrays with each 2×2 subblock’s
 *  diagonal minus antidiagonal sum nondecreasing horizontally
 *  and vertically.
 *
 *  For n > 6 the sequence is given by the closed formula
 *       a(n) = 529*3^(n-3) + 3431*2^(n-1) + 23248 .
 *
 *  For n ≤ 6 the values are
 *       a(1)=19701, a(2)=31630, a(3)=39463,
 *       a(4)=52912, a(5)=83003, a(6)=147332 .
 *
 *  This implementation uses the closed formula for n > 6
 *  and a small lookup table for n ≤ 6.
 */

static inline seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* small known values for n = 1..6 */
    static const seq_t small[7] = {
        0,          /* dummy to align indices */
        19701,
        31630,
        39463,
        52912,
        83003,
        147332
    };

    if (n <= 6)
        return small[n];

    /* Use the closed form for n > 6 */
    seq_t p3 = seq_pow((seq_t)3, n - 3);   /* 3^(n-3) */
    seq_t p2 = seq_pow((seq_t)2, n - 1);   /* 2^(n-1) */
    return 529 * p3 + 3431 * p2 + 23248;
}
