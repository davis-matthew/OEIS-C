/*
 * A341441  Total number of triangles visible in a regular (2n+1)-gon with all
 *          diagonals drawn.
 * https://oeis.org/A341441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1)*(2*n-1)*(2*n^3+21*n^2-2*n+9)/90
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – offset of sequence starts at 1 */

    seq_t n_seq = n;                /* ensure use of seq_t */
    seq_t f1 = n_seq;               /* n */
    seq_t f2 = 2 * n_seq + 1;       /* 2n+1 */
    seq_t f3 = 2 * n_seq - 1;       /* 2n-1 */

    /* compute 2*n^3 + 21*n^2 - 2*n + 9 */
    seq_t n_sq = n_seq * n_seq;
    seq_t n_cu = n_sq * n_seq;
    seq_t f4 = 2 * n_cu + 21 * n_sq - 2 * n_seq + 9;

    seq_t denom = 90;   /* split 90 into its prime factors during reduction */

    seq_t g;

    g = gcd_seq_t(f1, denom);
    if (g > 1) { f1 /= g; denom /= g; }

    g = gcd_seq_t(f2, denom);
    if (g > 1) { f2 /= g; denom /= g; }

    g = gcd_seq_t(f3, denom);
    if (g > 1) { f3 /= g; denom /= g; }

    g = gcd_seq_t(f4, denom);
    if (g > 1) { f4 /= g; denom /= g; }

    /* After removing all common factors, denom should be 1 */
    seq_t result = f1;
    result *= f2;
    result *= f3;
    result *= f4;

    if (denom != 1)            /* safety – should never happen for valid n */
        result /= denom;

    return result;
}
