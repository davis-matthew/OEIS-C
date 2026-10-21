/*
 * A295914  Number of n X 4 0..1 arrays with each 1 adjacent to 0 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A295914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295914
   Recurrence: a(n) = 2*a(n-1) + 7*a(n-2) + 6*a(n-3)
                 - 3*a(n-4) - 4*a(n-5) + a(n-6)
   Initial values for n = 1..6:
     a(1)=8,  a(2)=28,  a(3)=115,  a(4)=467,
     a(5)=1880, a(6)=7604                                       */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* not part of the sequence */

    /* values[0]..values[5] hold a(n-6) .. a(n-1) for the current n */
    seq_t values[6] = {
        (seq_t)8,    /* a(1) */
        (seq_t)28,   /* a(2) */
        (seq_t)115,  /* a(3) */
        (seq_t)467,  /* a(4) */
        (seq_t)1880, /* a(5) */
        (seq_t)7604  /* a(6) */
    };

    if (n <= 6) {
        return values[n - 1];
    }

    /* compute iteratively from n=7 up to the requested n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)2 * values[5]           /* a(i-1) */
                   + (seq_t)7 * values[4]           /* a(i-2) */
                   + (seq_t)6 * values[3]           /* a(i-3) */
                   - (seq_t)3 * values[2]           /* a(i-4) */
                   - (seq_t)4 * values[1]           /* a(i-5) */
                   + values[0];                    /* a(i-6) */

        /* shift: a(i-6) becomes a(i-5), ..., a(i-1) becomes a(i) */
        values[0] = values[1];
        values[1] = values[2];
        values[2] = values[3];
        values[3] = values[4];
        values[4] = values[5];
        values[5] = next;
    }

    return values[5];   /* a(n) */
}
