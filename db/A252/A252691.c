/*
 * A252691  Number of (n+2) X (4+2) 0..3 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and in every diagonal and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A252691: Number of (n+2) X 4+2 0..3 arrays with special row/column/diagonal constraints.
 * Recurrence (for n > 11):
 *   a(n) = 5*a(n-1) - 4*a(n-2) -11*a(n-3) + 9*a(n-4) + 12*a(n-5) + 12*a(n-6) 
 *          -34*a(n-7) - 8*a(n-8) + 28*a(n-9) - 8*a(n-10)
 * Initial values:
 *   a(1)=394, a(2)=519, a(3)=407, a(4)=1480, a(5)=2048, a(6)=6246,
 *   a(7)=12740, a(8)=35128, a(9)=84680, a(10)=225000, a(11)=573968
 */
seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        (seq_t)394,   (seq_t)519,   (seq_t)407,   (seq_t)1480,
        (seq_t)2048,  (seq_t)6246,  (seq_t)12740, (seq_t)35128,
        (seq_t)84680, (seq_t)225000,(seq_t)573968
    };

    if (n <= 11) {
        return init[n-1];
    }

    /* buffer of the last 11 terms:
     * arr[0] = a(k-11), arr[1] = a(k-10), ..., arr[10] = a(k-1)   (k current index)
     */
    seq_t arr[11];
    for (idx_t i = 0; i < 11; ++i) {
        arr[i] = init[i];
    }

    for (idx_t k = 12; k <= n; ++k) {
        /* compute a(k) using the recurrence, indexing relative to arr[0] = a(k-11) */
        seq_t pos = (seq_t)5 * arr[10]   /* a(k-1) */
                 + (seq_t)9 * arr[7]    /* a(k-4) */
                 + (seq_t)12 * arr[6]   /* a(k-5) */
                 + (seq_t)12 * arr[5]   /* a(k-6) */
                 + (seq_t)28 * arr[2];  /* a(k-9) */

        seq_t neg = (seq_t)4 * arr[9]    /* a(k-2) */
                 + (seq_t)11 * arr[8]   /* a(k-3) */
                 + (seq_t)34 * arr[4]   /* a(k-7) */
                 + (seq_t)8  * arr[3]   /* a(k-8) */
                 + (seq_t)8  * arr[1];  /* a(k-10) */

        seq_t next = pos - neg;          /* pos ≥ neg by the recurrence */

        /* shift buffer left by one and store next term */
        for (idx_t i = 0; i < 10; ++i) {
            arr[i] = arr[i + 1];
        }
        arr[10] = next;
    }

    return arr[10];
}
