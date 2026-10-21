/*
 * A278446  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 43", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) + 4*a(n-4) - 8*a(n-5) -
 *            4*a(n-6) + 8*a(n-7) with initial values a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=11, a(4)=20, a(5)=43, a(6)=80
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0..6 */
    static const seq_t init[7] = {1, 2, 4, 11, 20, 43, 80};
    /* coefficients for the recurrence */
    static const int coeff[7] = { 2,  1, -2,  4, -8, -4,  8 };

    if (n < 7)
        return init[n];

    /* circular buffer holding the last seven terms:
       buf[0] = a(n-7), ... , buf[6] = a(n-1) */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t step = 7; step <= n; ++step)
    {
        seq_t pos = 0; /* sum of positive contributions */
        seq_t neg = 0; /* sum of negative contributions */

        /* compute new value using the recurrence */
        for (int k = 0; k < 7; ++k)
        {
            int c = coeff[k];
            seq_t term = buf[6 - k] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                pos += term;
            else
                neg += term;
        }
        seq_t new_val = pos - neg; /* subtraction is safe because result is non‑negative */

        /* shift buffer: remove oldest term (buf[0]) */
        for (int i = 0; i < 6; ++i)
            buf[i] = buf[i + 1];
        buf[6] = new_val;
    }
    return buf[6];
}
