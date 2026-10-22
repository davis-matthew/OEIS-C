/*
 * A337285  a(n) = Sum_{i=1..n} (i-1)^2*T(i)^2, where T(i) = A000073(i) is the
 *          i-th tribonacci number.
 * https://oeis.org/A337285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (i-1)^2*T(i)^2, where T(i) is the i‑th
 *            tribonacci number (T(1)=0, T(2)=1, T(3)=1,
 *            T(i)=T(i-1)+T(i-2)+T(i-3) for i≥4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t t1 = 0;   /* T(1) */
    seq_t t2 = 1;   /* T(2) */
    seq_t t3 = 1;   /* T(3) */

    for (idx_t i = 1; i <= n; i++)
    {
        seq_t ti;
        if (i == 1)
            ti = t1;
        else if (i == 2)
            ti = t2;
        else if (i == 3)
            ti = t3;
        else
        {
            ti = t1 + t2 + t3;
            t1 = t2;
            t2 = t3;
            t3 = ti;
        }

        seq_t k = (seq_t)(i - 1);          /* (i-1) */
        seq_t term = k * k;                /* (i-1)^2 */
        term = term * ti;                  /* * T(i) */
        term = term * ti;                  /* * T(i) again */
        sum += term;
    }

    return sum;
}
