/*
 * A270724  a(n) = ((n+2)/2)*Sum_{k=0..n/2} (Sum_{i=0..n-2*k}
 *          (binomial(k+1,n-2*k-i)*binomial(k+i,k))/(k+1)*C(k)), where C(k) is
 *          Catalan numbers.
 * https://oeis.org/A270724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the Bostan–Mori recurrence derived from the
 *            binomial sum in OEIS A270724. The first eight terms are a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=10, a(4)=20, a(5)=42, a(6)=93, a(7)=213.
 *            For n≥8, (n+2)a(n) = (n+2)a(n-1)+(7n-6)a(n-2)-10a(n-3)
 *            -(13n-32)a(n-4)-(5n-32)a(n-5)+(11n-52)a(n-6) +4(n-6)a(n-...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..7 */
    static const seq_t base[8] = {1, 3, 5, 10, 20, 42, 93, 213};

    if (n < 8)
        return base[n];

    /* Sliding window of the last 8 terms: a(n-8)...a(n-1) */
    seq_t a0 = base[0];
    seq_t a1 = base[1];
    seq_t a2 = base[2];
    seq_t a3 = base[3];
    seq_t a4 = base[4];
    seq_t a5 = base[5];
    seq_t a6 = base[6];
    seq_t a7 = base[7];

    for (idx_t m = 8; m <= n; ++m)
    {
        seq_t num = (seq_t)(m + 2) * a7;
        num += (seq_t)(7 * m - 6) * a6;
        num -= (seq_t)10 * a5;
        num -= (seq_t)(13 * m - 32) * a4;
        num -= (seq_t)(5 * m - 32) * a3;
        num += (seq_t)(11 * m - 52) * a2;
        num += (seq_t)4 * (m - 6) * a1;
        num -= (seq_t)4 * (m - 7) * a0;

        seq_t next = num / (m + 2);

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = next;
    }
    return a7;
}
