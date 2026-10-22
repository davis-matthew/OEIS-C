/*
 * A389534  Length of shortest loop (in Chebyshev distance) that touches all
 *          cells in an n X n grid.
 * https://oeis.org/A389534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   For n < 13 the values are fixed:
 *            0,0,0,4,8,12,16,22,28,35,42,49,56 For n ≥ 13 the length equals:
 *            - if n ≡ 0 (mod 6): 2*n + n*(n-4)/3 - if n ≡ 1 (mod 6): 2*n + 6
 *            + (n*(n-4)-12)/3 - if n ≡ 2 (mod 6): 2*n + 10 + (n*(n-4)-20)/3 -
 *            if n ≡ 3 (mod 6): 2*n + 6 + (n*(n-4)-12)/3 - if n ≡ 4 (mod 6):
 *            2*n + 6...
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small values handled explicitly. */
    static const seq_t small[13] = {
        0, 0, 0, 4, 8, 12, 16, 22, 28, 35, 42, 49, 56
    };

    if (n < 13)
        return small[n];

    idx_t r      = n % 6;
    seq_t N      = (seq_t)n;
    seq_t result = 0;

    switch (r) {
        case 0:
            result = (seq_t)2 * N + (N * (N - (seq_t)4)) / (seq_t)3;
            break;
        case 1:
            result = (seq_t)2 * N + (seq_t)6 +
                    ((N * (N - (seq_t)4)) - (seq_t)12) / (seq_t)3;
            break;
        case 2:
            result = (seq_t)2 * N + (seq_t)10 +
                    ((N * (N - (seq_t)4)) - (seq_t)20) / (seq_t)3;
            break;
        case 3:
        case 4:
            result = (seq_t)2 * N + (seq_t)6 +
                    ((N * (N - (seq_t)4)) - (seq_t)12) / (seq_t)3;
            break;
        case 5:
            result = (seq_t)2 * N + (seq_t)4 +
                    ((N * (N - (seq_t)4)) - (seq_t)8) / (seq_t)3;
            break;
    }

    return result;
}
