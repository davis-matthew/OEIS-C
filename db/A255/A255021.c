/*
 * A255021  Number of (n+2) X (2+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 or 1 and no antidiagonal sum 0 or 1 and no row sum
 *          0 or 1 and no column sum 0 or 1.
 * https://oeis.org/A255021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-2)+6*a(n-3)+5*a(n-4)+a(n-5)-2*a(n-6)-2*a(n-7)-a(n-8);
 *            Initial values: a(1)=54 a(2)=134 a(3)=405 a(4)=1121 a(5)=3015
 *            a(6)=8338 a(7)=23077 a(8)=63446 a(9)=174690
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset. Sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;

    /* Base values for n = 1 .. 9.  Stored in type idx_t so that they
       can be safely cast to seq_t when needed. */
    static const idx_t base_vals[9] = {
        54, 134, 405, 1121, 3015, 8338, 23077, 63446, 174690
    };

    if (n <= 9) {
        return (seq_t)base_vals[n - 1];
    }

    /* Sliding window of the last 9 terms of the sequence.
       t[0] holds a(n-8), t[8] holds a(n-1) for the current iteration. */
    seq_t t[9];
    for (int i = 0; i < 9; ++i) {
        t[i] = (seq_t)base_vals[i];
    }

    /* Compute terms from n = 10 up to the required n. */
    for (idx_t k = 10; k <= n; ++k) {
        /* Compute addition part and subtraction part separately to avoid
           unsigned underflow. */
        seq_t sum_plus =  t[8]                                  /* a(k-1) */
                       + (seq_t)2 * t[7]                       /* 2*a(k-2) */
                       + (seq_t)6 * t[6]                       /* 6*a(k-3) */
                       + (seq_t)5 * t[5]                       /* 5*a(k-4) */
                       + t[4];                                 /* a(k-5) */

        seq_t sum_minus = (seq_t)2 * t[3]                       /* 2*a(k-6) */
                        + (seq_t)2 * t[2]                       /* 2*a(k-7) */
                        + t[1];                                 /* a(k-8) */

        seq_t new_val = sum_plus - sum_minus;

        /* Shift the window left by one position. */
        for (int i = 0; i < 8; ++i) {
            t[i] = t[i + 1];
        }
        t[8] = new_val;
    }

    return t[8];
}
