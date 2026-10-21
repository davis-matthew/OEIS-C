/*
 * A253344  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 6*a(n-1) - 5*a(n-2) - 12*a(n-3) +
 *            12*a(n-4) (n>6) with initial values a(1)=47, a(2)=173, a(3)=735,
 *            a(4)=3192, a(5)=13917, a(6)=60779
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not in the defined domain */

    /* Initial six terms */
    seq_t a1 = (seq_t)47ULL;
    seq_t a2 = (seq_t)173ULL;
    seq_t a3 = (seq_t)735ULL;
    seq_t a4 = (seq_t)3192ULL;
    seq_t a5 = (seq_t)13917ULL;
    seq_t a6 = (seq_t)60779ULL;

    switch (n) {
        case 1: return a1;
        case 2: return a2;
        case 3: return a3;
        case 4: return a4;
        case 5: return a5;
        case 6: return a6;
    }

    /* Compute terms a(7)..a(n) iteratively, keeping only the last five terms. */
    seq_t cur;
    for (idx_t k = 7; k <= n; ++k) {
        cur = a6 * (seq_t)6ULL
            - a5 * (seq_t)5ULL
            - a4 * (seq_t)12ULL
            + a3 * (seq_t)12ULL;

        /* shift window: drop a3, move a4->a3, a5->a4, a6->a5, cur->a6 */
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = cur;
    }

    return cur;
}
