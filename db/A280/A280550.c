/*
 * A280550  Number of n X 4 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A280550 is given by the empirical linear recurrence a(n) =
 *            6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5) -3*a(n-6)
 *            +6*a(n-7) - a(n-9) for n > 11 Initial values: a(1)=1, a(2)=34,
 *            a(3)=144, a(4)=382, a(5)=832, a(6)=1666, a(7)=3182, a(8)=5886,
 *            a(9)=10680, a(10)=19122, a(11)=33920. The function...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 76, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms a(1)..a(11) */
    static const seq_t init[11] = {
        (seq_t)1,      /* a(1) */
        (seq_t)34,     /* a(2) */
        (seq_t)144,    /* a(3) */
        (seq_t)382,    /* a(4) */
        (seq_t)832,    /* a(5) */
        (seq_t)1666,   /* a(6) */
        (seq_t)3182,   /* a(7) */
        (seq_t)5886,   /* a(8) */
        (seq_t)10680,  /* a(9) */
        (seq_t)19122,  /* a(10) */
        (seq_t)33920   /* a(11) */
    };

    /* trivial cases */
    if (n < 1) {
        return (seq_t)0;
    }
    if (n <= 11) {
        return init[n - 1];
    }

    /* state holds a(i-9)..a(i-1) for the current i.  
       Initially set i = 12, and state contains a(3) .. a(11)  */
    seq_t state[9];
    for (int i = 0; i < 9; ++i) {
        state[i] = init[i + 2];      /* init[2] = a(3), ..., init[10] = a(11) */
    }

    idx_t cur = 12; /* next value to compute */

    while (cur <= n) {
        seq_t pos = (seq_t)6 * state[8] +        /* 6*a(i-1) */
                    (seq_t)5 * state[6] +        /* 5*a(i-3) */
                    (seq_t)12 * state[5] +       /* 12*a(i-4) */
                    (seq_t)6 * state[2];        /* 6*a(i-7) */

        seq_t neg = (seq_t)12 * state[7] +       /* 12*a(i-2) */
                    (seq_t)12 * state[4] +       /* 12*a(i-5) */
                    (seq_t)3 * state[3] +        /* 3*a(i-6) */
                    state[0];                    /* a(i-9) */

        seq_t next = pos - neg;                  /* a(i) */

        /* shift state left and insert next at the end */
        for (int j = 0; j < 8; ++j) {
            state[j] = state[j + 1];
        }
        state[8] = next;
        ++cur;
    }

    return state[8];
}
