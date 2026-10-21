/*
 * A249845  Number of length 1+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + (5/2)*n^4 + (10/3)*n^3 + (5/2)*n^2 + (2/3)*n
 *            Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4)
 *            + 6*a(n-5) - a(n-6), n>6 Initial values: a(1)=10, a(2)=110,
 *            a(3)=560, a(4)=1920, a(5)=5170, a(6)=11830
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset is 1; for n < 1 the behavior is irrelevant */
    static const seq_t init[6] = {10, 110, 560, 1920, 5170, 11830};

    if (n <= 6)
        return init[n - 1];

    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t a6 = init[5];
    seq_t next = 0;

    for (idx_t i = 7; i <= n; ++i) {
        next = 6 * a6 - 15 * a5 + 20 * a4 - 15 * a3 + 6 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}
