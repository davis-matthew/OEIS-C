/*
 * A308265  Sum of the largest parts in the partitions of n into 3 parts.
 * https://oeis.org/A308265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) - 4*a(n-5) - a(n-6) +
 *            2*a(n-7) + 2*a(n-8) - a(n-10) (n > 10) Initial values: a(1) = 0,
 *            a(2) = 0, a(3) = 1, a(4) = 2, a(5) = 5, a(6) = 9, a(7) = 15,
 *            a(8) = 22, a(9) = 34, a(10) = 45
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and the first ten terms specially. */
    if (n <= 10) {
        switch ((idx_t)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)1;
            case 4: return (seq_t)2;
            case 5: return (seq_t)5;
            case 6: return (seq_t)9;
            case 7: return (seq_t)15;
            case 8: return (seq_t)22;
            case 9: return (seq_t)34;
            case 10:return (seq_t)45;
        }
    }

    /* Buffer that holds the ten preceding values for the recurrence.
       buf[0] = a(i-10), buf[1] = a(i-9), …, buf[9] = a(i-1).   */
    seq_t buf[10];

    /* Initialize the buffer with the known first ten terms. */
    buf[0] = (seq_t)0;   /* a(1) */
    buf[1] = (seq_t)0;   /* a(2) */
    buf[2] = (seq_t)1;   /* a(3) */
    buf[3] = (seq_t)2;   /* a(4) */
    buf[4] = (seq_t)5;   /* a(5) */
    buf[5] = (seq_t)9;   /* a(6) */
    buf[6] = (seq_t)15;  /* a(7) */
    buf[7] = (seq_t)22;  /* a(8) */
    buf[8] = (seq_t)34;  /* a(9) */
    buf[9] = (seq_t)45;  /* a(10) */

    /* Compute terms a(11) … a(n) iteratively using the recurrence. */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t next = 0;
        next = 2 * buf[8]    /* 2*a(i-2) */ ;
        next += 2 * buf[7];  /* 2*a(i-3) */
        next -= buf[6];      /* -a(i-4) */
        next -= 4 * buf[5];  /* -4*a(i-5) */
        next -= buf[4];      /* -a(i-6) */
        next += 2 * buf[3];  /* +2*a(i-7) */
        next += 2 * buf[2];  /* +2*a(i-8) */
        next -= buf[0];      /* -a(i-10) */

        /* Shift the buffer one step left: a(i-9) becomes a(i-10), etc. */
        for (idx_t j = 0; j < 9; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[9] = next;  /* buf[9] now holds a(i) */
    }

    /* After the loop, buf[9] holds a(n). */
    return buf[9];
}
