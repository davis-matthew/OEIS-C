/*
 * A299062  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2) + 14*a(n-3) + 11*a(n-4) * - 4*a(n-5)
 *            + 3*a(n-6) + 2*a(n-7) - 5*a(n-8) - 2*a(n-9) for n > 10 * Initial
 *            values (n = 1 … 10): * a(1) = 1 * a(2) = 18 * a(3) = 141 * a(4)
 *            = 993 * a(5) = 7330 * a(6) = 54106 * a(7) = 398654 * a(8) =
 *            2937795 * a(9) = 21650600 * a(10)= 159556133
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; for completeness return 0 */
        return 0;
    }

    /* Initial 10 terms of the sequence */
    static const seq_t init[10] = {
        (seq_t)1,          /* a(1) */
        (seq_t)18,         /* a(2) */
        (seq_t)141,        /* a(3) */
        (seq_t)993,        /* a(4) */
        (seq_t)7330,       /* a(5) */
        (seq_t)54106,      /* a(6) */
        (seq_t)398654,     /* a(7) */
        (seq_t)2937795,    /* a(8) */
        (seq_t)21650600,   /* a(9) */
        (seq_t)159556133   /* a(10) */
    };

    if (n <= 10) {
        return init[n - 1];
    }

    /* Buffer holding the last 9 values: a(n-9) … a(n-1) */
    seq_t last[9];
    for (int i = 0; i < 9; ++i) {
        /* indices 1..9 correspond to a(2) … a(10) */
        last[i] = init[1 + i];
    }

    /* Compute terms from n = 11 up to the requested n */
    for (idx_t i = 11; i <= n; ++i) {
        /* pos = 6*a(n-1) + 8*a(n-2) + 14*a(n-3) + 11*a(n-4)
         *        + 3*a(n-6) + 2*a(n-7)
         * neg = 4*a(n-5) + 5*a(n-8) + 2*a(n-9)            */
        seq_t pos = (seq_t)6 * last[8]
                  + (seq_t)8 * last[7]
                  + (seq_t)14 * last[6]
                  + (seq_t)11 * last[5]
                  + (seq_t)3  * last[3]
                  + (seq_t)2  * last[2];
        seq_t neg = (seq_t)4 * last[4]
                  + (seq_t)5 * last[1]
                  + (seq_t)2 * last[0];
        seq_t next = pos - neg;        /* recurrence result */

        /* Shift buffer left and insert the new value as last element */
        for (int k = 0; k < 8; ++k) {
            last[k] = last[k + 1];
        }
        last[8] = next;
    }

    /* After loop, last[8] holds a(n) */
    return last[8];
}
