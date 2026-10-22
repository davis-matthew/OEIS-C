/*
 * A356251  a(n) = n*(n+1)*(n+2)*(n+3)*(2*n+1)/12.
 * https://oeis.org/A356251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(2*n+1)/12
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    seq_t r;
    while (b != 0) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) = 0 */

    seq_t n0 = (seq_t)n;
    seq_t terms[5] = { n0, n0 + 1, n0 + 2, n0 + 3, 2 * n0 + 1 };

    seq_t result = 1;
    seq_t denom  = 12;   /* the factor to divide by */

    for (int i = 0; i < 5; ++i) {
        seq_t t = terms[i];

        /* cancel common factor between this term and the remaining denominator */
        seq_t g = gcd_seq(t, denom);
        t      = t / g;
        denom  = denom / g;

        /* further cancel any common factor between the current result
           and what remains of the denominator (helps keep numbers small) */
        if (denom > 1) {
            seq_t g2 = gcd_seq(result, denom);
            result   = result / g2;
            denom    = denom / g2;
        }

        result = result * t;
    }

    /* any residual denominator (should divide result exactly) */
    if (denom > 1)
        result = result / denom;

    return result;
}
