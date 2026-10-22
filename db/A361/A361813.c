/*
 * A361813  Expansion of 1/sqrt(1 - 4*x*(1+x)^4).
 * https://oeis.org/A361813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k) * binomial(4*k,n-k)
 *            Recurrence (for n > 4): n*a(n) = 2*((2*n-1)*a(n-1) +
 *            4*(2*n-2)*a(n-2) + 6*(2*n-3)*a(n-3) + 4*(2*n-4)*a(n-4) +
 *            (2*n-5)*a(n-5)). Base values: a(0)=1, a(1)=2, a(2)=14, a(3)=80,
 *            a(4)=486
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)80;
    if (n == 4) return (seq_t)486;

    /* a[0]=a(n-1), a[1]=a(n-2), ..., a[4]=a(n-5) */
    seq_t a[5];
    a[0] = (seq_t)486;   /* a(4) */
    a[1] = (seq_t)80;    /* a(3) */
    a[2] = (seq_t)14;    /* a(2) */
    a[3] = (seq_t)2;     /* a(1) */
    a[4] = (seq_t)1;     /* a(0) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t term = 0;
        /* (2*i-1)*a[i-1] */
        term += (seq_t)(2 * i - 1) * a[0];
        /* 4*(2*i-2)*a[i-2] */
        term += (seq_t)4 * ((seq_t)(2 * i - 2) * a[1]);
        /* 6*(2*i-3)*a[i-3] */
        term += (seq_t)6 * ((seq_t)(2 * i - 3) * a[2]);
        /* 4*(2*i-4)*a[i-4] */
        term += (seq_t)4 * ((seq_t)(2 * i - 4) * a[3]);
        /* (2*i-5)*a[i-5] */
        term += (seq_t)(2 * i - 5) * a[4];
        term *= (seq_t)2;          /* multiply whole sum by 2 */

        /* a(i) = term / i */
        seq_t ai = term / (seq_t)i;

        /* Shift the window for next iteration */
        a[4] = a[3];
        a[3] = a[2];
        a[2] = a[1];
        a[1] = a[0];
        a[0] = ai;
    }

    return a[0];   /* a(n) is now in a[0] */
}
