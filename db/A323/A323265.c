/*
 * A323265  The sequence denoted by p_n used in the calculation of A323260.
 * https://oeis.org/A323265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A323265
    Recurrence: a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4) - a(n-5)
    Initial values: a(1)=0, a(2)=2, a(3)=6, a(4)=26, a(5)=102
*/
seq_t a(idx_t n)
{
    /* Constant base terms */
    static const seq_t base[5] = {(seq_t)0, (seq_t)2, (seq_t)6,
                                  (seq_t)26, (seq_t)102};

    if (n <= 5)
        return base[n - 1];

    /* Sliding window for the last five values */
    seq_t a0 = base[0]; /* a(n-5) */
    seq_t a1 = base[1]; /* a(n-4) */
    seq_t a2 = base[2]; /* a(n-3) */
    seq_t a3 = base[3]; /* a(n-2) */
    seq_t a4 = base[4]; /* a(n-1) */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Apply recurrence using only seq_t arithmetic */
        next = 5 * a4 - 3 * a3 - 5 * a2 + 7 * a1 - a0;

        /* Shift window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4; /* a(n) */
}
