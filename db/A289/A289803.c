/*
 * A289803  p-INVERT of the even bisection (A001906) of the Fibonacci numbers,
 *          where p(S) = 1 - S - S^2.
 * https://oeis.org/A289803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 13*a(n-2) + 7*a(n-3) - a(n-4) for n ≥ 4; a(0)
 *            = 1; a(1) = 5; a(2) = 23; a(3) = 103;
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = {
        (seq_t)1,   /* a(0) */
        (seq_t)5,   /* a(1) */
        (seq_t)23,  /* a(2) */
        (seq_t)103  /* a(3) */
    };
    if (n < 4) return init[n];

    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 7 * a3 - 13 * a2 + 7 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
