/*
 * A373741  Expansion of e.g.f. exp(x^2/2 * (1 + x)^3).
 * https://oeis.org/A373741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} binomial(3k, n-2k) / (2^k * k!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* or   a(n) = ((n-1) * ( 2*a(n-2) + 9*(n-2)*a(n-3) + 12*(n-2)*(n-3)*a(n-4) + 5*(n-2)*(n-3)*(n-4)*a(n-5) )) / 2; */
/* initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=9, a(4)=39, a(5)=150 */

seq_t a(idx_t n)
{
    /* handle the first six values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)39;
    if (n == 5) return (seq_t)150;

    /* circular buffer for the last seven computed values */
    seq_t buf[7] = {
        (seq_t)1,   /* a(0) */
        (seq_t)0,   /* a(1) */
        (seq_t)1,   /* a(2) */
        (seq_t)9,   /* a(3) */
        (seq_t)39,  /* a(4) */
        (seq_t)150, /* a(5) */
        (seq_t)0    /* placeholder for a(6) onwards */
    };

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t v2 = buf[(i - 2) % 7];
        seq_t v3 = buf[(i - 3) % 7];
        seq_t v4 = buf[(i - 4) % 7];
        seq_t v5 = buf[(i - 5) % 7];

        /* compute the inner sum of the recurrence */
        seq_t sum = (seq_t)2 * v2;
        sum += (seq_t)9 * (seq_t)(i - 2) * v3;
        sum += (seq_t)12 * (seq_t)(i - 2) * (seq_t)(i - 3) * v4;
        sum += (seq_t)5 * (seq_t)(i - 2) * (seq_t)(i - 3) * (seq_t)(i - 4) * v5;

        /* apply the outer factor ((i-1) / 2) but perform multiplication first */
        seq_t val = ((seq_t)(i - 1) * sum) / 2;

        buf[i % 7] = val;
    }

    return buf[n % 7];
}
