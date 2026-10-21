/*
 * A281238  Solutions y to the negative Pell equation y^2 = 72*x^2 - 73728
 *          with x,y >= 0.
 * https://oeis.org/A281238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) (for n > 2) Initial values: a(1) = 0,
 *            a(2) = 768
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A281238.
   No external dependencies are required. */
seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }
    if (n == 2) {
        return 768;
    }

    seq_t a_prev = 0;      /* a(n-2) */
    seq_t a_cur  = 768;    /* a(n-1) */

    /* Iterate from i = 3 up to n, computing a(i) = 6*a(i-1) - a(i-2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_next = 6 * a_cur - a_prev;
        a_prev = a_cur;
        a_cur  = a_next;
    }

    return a_cur;
}
