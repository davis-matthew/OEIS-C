/*
 * A351805  a(n) = Sum_{1 <= i < j <= n} j^5*i^5.
 * https://oeis.org/A351805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(n + 1)*(44*n^9 + 120*n^8 - 132*n^7 - 540*n^6 +
 *            99*n^5 + 912*n^4 - 11*n^3 - 672*n^2 + 120)/3168
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t N    = (seq_t)n;
    seq_t n2   = N * N;
    seq_t n3   = n2 * N;
    seq_t n4   = n3 * N;
    seq_t n5   = n4 * N;
    seq_t n6   = n5 * N;
    seq_t n7   = n6 * N;
    seq_t n8   = n7 * N;
    seq_t n9   = n8 * N;

    seq_t pos = 44*n9 + 120*n8 + 99*n5 + 912*n4 + 120;
    seq_t neg = 132*n7 + 540*n6 + 11*n3 + 672*n2;
    seq_t P   = pos - neg;

    seq_t factor = N * (N - 1) * (N + 1);
    seq_t denom  = 3168;  /* 2^5 * 3^2 * 11 */

    /* Reduce the 2^5 factor */
    for (int i = 0; i < 5; ++i) {
        if (denom % 2 == 0) {
            if (factor % 2 == 0) { factor /= 2; denom /= 2; }
            else if (P % 2 == 0) { P   /= 2; denom /= 2; }
        }
    }
    /* Reduce the 3^2 factor */
    for (int i = 0; i < 2; ++i) {
        if (denom % 3 == 0) {
            if (factor % 3 == 0) { factor /= 3; denom /= 3; }
            else if (P % 3 == 0) { P   /= 3; denom /= 3; }
        }
    }
    /* Reduce the 11 factor */
    if (denom % 11 == 0) {
        if (factor % 11 == 0) { factor /= 11; denom /= 11; }
        else if (P % 11 == 0) { P   /= 11; denom /= 11; }
    }

    /* After all reductions denom should be 1 */
    seq_t result = factor * P;
    return result;
}
