/*
 * A370696  Expansion of e.g.f. exp(x^3)/(1-x).
 * https://oeis.org/A370696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} 1/k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prev = 1; /* a(0) */
    seq_t P = 0;    /* Product used when n % 3 == 0 */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t next = prev * (seq_t)i;
        if (i % 3 == 0)
        {
            idx_t m = i / 3;
            if (m == 1)
            {
                P = (seq_t)2; /* For i = 3: product = 2 */
            }
            else
            {
                /* Update P from the previous multiple
                   P_m = (P_{m-1} / m) * (i-3)*(i-2)*(i-1) */
                P = P / (seq_t)m;
                P = P * (seq_t)(i - 3);
                P = P * (seq_t)(i - 2);
                P = P * (seq_t)(i - 1);
            }
            next += (seq_t)i * P;
        }
        prev = next;
    }
    return prev;
}
