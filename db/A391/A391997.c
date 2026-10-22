/*
 * A391997  a(n) = Sum_{k=0..n} floor((k/2)^2)*n^2. Row sums of A391996.
 * https://oeis.org/A391997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * sum_{k=0}^{n} floor((k/2)^2). Sum of squares:
 *            sumsq(m) = m*(m+1)*(2*m+1)/6
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sum of squares up to m (inclusive) */
static inline seq_t sumsq(idx_t m)
{
    seq_t a = (seq_t)m;
    seq_t b = a + 1;
    seq_t c = (a << 1) + 1; /* 2*a+1 */
    return a * b * c / 6;
}

/* a(n) as defined in OEIS A391997 */
seq_t a(idx_t n)
{
    idx_t M = n >> 1;                /* floor(n/2) */
    seq_t sum;                       /* S(n) */

    if ((n & 1) == 0) { /* n even: n = 2*M */
        seq_t msq   = sumsq(M);      /* Sum_{m=0}^{M}   m^2   */
        seq_t msq1  = (M > 0) ? sumsq(M - 1) : 0; /* Sum_{m=0}^{M-1} m^2 */
        seq_t part  = (seq_t)M * (M - 1) / 2;      /* M*(M-1)/2 */
        sum = msq + msq1 + part;
    } else { /* n odd: n = 2*M+1 */
        seq_t msq   = sumsq(M);      /* Sum_{m=0}^{M}   m^2   */
        seq_t part  = (seq_t)M * (M + 1) / 2;      /* M*(M+1)/2 */
        sum = (msq << 1) + part;     /* 2*msq + part   */
    }

    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
    return nn * sum;                  /* a(n) = n^2 * S(n) */
}
