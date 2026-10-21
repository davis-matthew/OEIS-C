/*
 * A265038  Partial sums of A009927.
 * https://oeis.org/A265038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 938, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=13, a(2)=63, a(3)=183, a(4)=401, a(5)=745,
   a(6)=1291, a(7)=2019, a(8)=2921, a(9)=4133, a(10)=5659, a(11)=7443,
   a(12)=9597, a(13)=12149, a(14)=15103.
   For n ≥ 15:
   a(n) = a(n-1) - a(n-2) + 3*a(n-3) - a(n-4) + a(n-5)
          - 2*a(n-6) - 2*a(n-7) + a(n-8) - a(n-9)
          + 3*a(n-10) - a(n-11) + a(n-12) - a(n-13);
   (Non‑noline: terms are all unsigned integers.) */

seq_t a(idx_t n)
{
    /* Initial terms a(0) … a(14). */
    static const seq_t first15[15] = {
        (seq_t)1,   (seq_t)13,   (seq_t)63,   (seq_t)183,
        (seq_t)401, (seq_t)745,  (seq_t)1291, (seq_t)2019,
        (seq_t)2921, (seq_t)4133,(seq_t)5659, (seq_t)7443,
        (seq_t)9597, (seq_t)12149,(seq_t)15103
    };

    /* Return if n is in the initial range */
    if (n < 15) {
        return first15[n];
    }

    /* Work array holding the last 15 computed values */
    seq_t vals[15];
    for (int i = 0; i < 15; ++i) {
        vals[i] = first15[i];
    }

    for (idx_t k = 15; k <= n; ++k) {
        seq_t next = vals[14] - vals[13] + 3 * vals[12] - vals[11]
                   + vals[10] - 2 * vals[9] - 2 * vals[8] + vals[7]
                   - vals[6] + 3 * vals[5] - vals[4] + vals[3] - vals[2];

        /* Shift left by one position and insert the new value */
        for (int i = 0; i < 14; ++i) {
            vals[i] = vals[i + 1];
        }
        vals[14] = next;
    }

    return vals[14];   /* This is a(n) after the loop */
}
