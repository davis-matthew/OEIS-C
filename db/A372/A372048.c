/*
 * A372048  The index of the largest Fibonacci number that divides the sum of
 *          Fibonacci numbers with indices 1 through n.
 * https://oeis.org/A372048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n >= 8, a(n) = a(n-4) + 2.
   Initial values (1‑indexed): a(1)=2, a(2)=3, a(3)=3, a(4)=2,
   a(5)=4, a(6)=5, a(7)=4, a(8)=4. */
seq_t a(idx_t n)
{
    /* lookup table for indices 1 through 11 */
    static const seq_t tbl[12] = {
        0,  /* unused index 0 */
        2,  /* a(1) */
        3,  /* a(2) */
        3,  /* a(3) */
        2,  /* a(4) */
        4,  /* a(5) */
        5,  /* a(6) */
        4,  /* a(7) */
        4,  /* a(8) */
        6,  /* a(9) */
        7,  /* a(10) */
        6   /* a(11) */
    };

    if (n <= 8)
        return tbl[n];

    /* For n >= 8 use a(n) = a(n-4) + 2 repeatedly */
    idx_t offset = n - 8;
    idx_t k = offset / 4;       /* number of 4‑step increments */
    idx_t mod = offset % 4;     /* which seed among 8..11 */
    idx_t base = 8 + mod;       /* seed index */

    return tbl[base] + (seq_t)2 * k;
}
