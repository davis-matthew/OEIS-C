/*
 * A255469  A255468(2^n-1).
 * https://oeis.org/A255469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A255469: a(0)=1, a(1)=6, a(2)=20, a(3)=84, a(4)=296,
   a(5)=1124, a(6)=4056, a(7)=15100, a(8)=55288.
   For n ≥ 9:
   a(n) = 5*a(n-1) - a(n-2) - 25*a(n-3) + 44*a(n-4) - 2*a(n-5)
          - 56*a(n-6) + 40*a(n-7) - 24*a(n-8) - 16*a(n-9); */
seq_t a(idx_t n)
{
    /* First nine values, as required by the generating function. */
    static const seq_t init[9] = {
        (seq_t)1,
        (seq_t)6,
        (seq_t)20,
        (seq_t)84,
        (seq_t)296,
        (seq_t)1124,
        (seq_t)4056,
        (seq_t)15100,
        (seq_t)55288
    };

    /* Handle indices below the offset. */
    if (n <= 8)
        return init[n];

    /* Working buffer holding a(n-9) .. a(n-1). */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[i];

    seq_t next;
    for (idx_t i = 9; i <= n; ++i)
    {
        next = (seq_t)5  * buf[8]
             - buf[7]
             - (seq_t)25 * buf[6]
             + (seq_t)44 * buf[5]
             - (seq_t)2  * buf[4]
             - (seq_t)56 * buf[3]
             + (seq_t)40 * buf[2]
             - (seq_t)24 * buf[1]
             - (seq_t)16 * buf[0];

        /* Shift buffer left (drop the oldest term). */
        for (int k = 0; k < 8; ++k)
            buf[k] = buf[k + 1];
        buf[8] = next;
    }

    return buf[8];
}
