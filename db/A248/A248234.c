/*
 * A248234  a(n) = floor(1/(zeta(5) - Sum_{h=1..n} 1/h^5)).
 * https://oeis.org/A248234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/(zeta(5) - Σ_{h=1..n} 1/h^5)) The recurrence is
 *            used to compute the terms efficiently
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 180, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 5*a(n-4)
           + 6*a(n-5) - 4*a(n-6) + a(n-7)   for n >= 10
    with initial values a(1)=27, a(2)=176, ..., a(9)=32879. */

seq_t a(idx_t n)
{
    /* Known first nine terms */
    static const seq_t seeds[10] = {
        0ULL,          /* index 0 unused */
        27ULL,   176ULL,  639ULL, 1706ULL, 3759ULL,
        7279ULL, 12842ULL, 21119ULL, 32879ULL
    };

    if (n <= 9)
        return seeds[(idx_t)n];

    /* Compute a(n) iteratively using the 7‑term recurrence.
       rev[0] holds a(i-1), rev[1] a(i-2), …, rev[6] a(i-7). */
    seq_t rev[7];
    rev[0] = seeds[9];
    rev[1] = seeds[8];
    rev[2] = seeds[7];
    rev[3] = seeds[6];
    rev[4] = seeds[5];
    rev[5] = seeds[4];
    rev[6] = seeds[3];

    seq_t cur = 0;

    for (idx_t i = 10; i <= n; ++i) {
        cur = 4 * rev[0]
            - 6 * rev[1]
            + 5 * rev[2]
            - 5 * rev[3]
            + 6 * rev[4]
            - 4 * rev[5]
            + rev[6];
        /* shift the ring buffer */
        rev[6] = rev[5];
        rev[5] = rev[4];
        rev[4] = rev[3];
        rev[3] = rev[2];
        rev[2] = rev[1];
        rev[1] = rev[0];
        rev[0] = cur;
    }

    return cur;
}
