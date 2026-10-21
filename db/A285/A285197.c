/*
 * A285197  Expansion of (1-6*x+11*x^2-5*x^3) / ((1-x)*(1-3*x)*(1-3*x+x^2)).
 * https://oeis.org/A285197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 13*a(n-3) - 3*a(n-4) (n>3) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = {1, 1, 2, 6};

    if (n < 4)
        return init[n];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t next;

    for (idx_t i = 4; i <= n; ++i)
    {
        next = 7 * a3 - 16 * a2 + 13 * a1 - 3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
