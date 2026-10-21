/*
 * A241055  Number of 2 X n 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A241055: Number of 2×n 0..3 arrays with the given local
   restrictions (mod 4).  The sequence satisfies the empirical
   recurrence (for n > 17)

        a(n) = 3*a(n-2) + 3*a(n-3) - 3*a(n-4) - 4*a(n-5) - 2*a(n-6)
             + 3*a(n-7) - a(n-9) + 2*a(n-11) - 2*a(n-12) + 2*a(n-15)

   Initial values (n = 1 … 17):

        a(1) = 2,  a(2) = 3,  a(3) = 2,  a(4) = 10,
        a(5) = 18, a(6) = 18, a(7) = 46, a(8) = 58,
        a(9) = 102, a(10)=173, a(11)=264, a(12)=487,
        a(13)=738, a(14)=1318, a(15)=2151, a(16)=3591, a(17)=6192
*/
seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[17] = {
        (seq_t)2,  (seq_t)3,  (seq_t)2,  (seq_t)10,
        (seq_t)18, (seq_t)18, (seq_t)46, (seq_t)58,
        (seq_t)102,(seq_t)173,(seq_t)264,(seq_t)487,
        (seq_t)738,(seq_t)1318,(seq_t)2151,(seq_t)3591,
        (seq_t)6192
    };

    if (n <= 17)
        return init[n-1];

    /* Buffer holding the last 15 terms a(n-15)..a(n-1) */
    seq_t buf[15];
    /* For the first iteration (n=18) the buffer must contain
       a(3) .. a(17).  init[0] = a(1) */
    for (idx_t i = 0; i < 15; ++i)
        buf[i] = init[2 + i];

    seq_t next = 0;

    for (idx_t cur = 18; cur <= n; ++cur)
    {
        /* Sum of the positive terms of the recurrence */
        seq_t pos_sum = 0;
        pos_sum += buf[13] + buf[13] + buf[13]; /* 3*a(n-2) */
        pos_sum += buf[12] + buf[12] + buf[12]; /* 3*a(n-3) */
        pos_sum += buf[8]  + buf[8]  + buf[8];  /* 3*a(n-7) */
        pos_sum += buf[4]  + buf[4];            /* 2*a(n-11) */
        pos_sum += buf[0]  + buf[0];            /* 2*a(n-15) */

        /* Sum of the negative terms of the recurrence */
        seq_t neg_sum = 0;
        neg_sum += buf[11] + buf[11] + buf[11]; /* 3*a(n-4) */
        {
            seq_t t = buf[10] + buf[10];        /* 2*a(n-5) */
            neg_sum += t + t;                  /* 4*a(n-5) */
        }
        neg_sum += buf[9] + buf[9];            /* 2*a(n-6) */
        neg_sum += buf[6];                     /* 1*a(n-9) */
        neg_sum += buf[3] + buf[3];            /* 2*a(n-12) */

        next = pos_sum;
        next -= neg_sum;                       /* a(cur) = pos_sum - neg_sum */

        /* Shift buffer left and append the new term */
        for (idx_t i = 0; i < 14; ++i)
            buf[i] = buf[i+1];
        buf[14] = next;
    }

    return next;
}
