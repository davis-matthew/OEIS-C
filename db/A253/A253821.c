/*
 * A253821  Indices of octagonal numbers (A000567) which are also centered
 *          triangular numbers (A005448).
 * https://oeis.org/A253821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-1)+1154*a(n-2)-1154*a(n-3)-a(n-4)+a(n-5);
   Initial values: a(1)=1, a(2)=181, a(3)=589, a(4)=208489, a(5)=679321 */

seq_t a(idx_t n)
{
    /* initial 5 terms */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)181,
        (seq_t)589,
        (seq_t)208489,
        (seq_t)679321
    };

    /* return directly for indices ≤5 */
    if (n <= 5) {
        return init[n - 1];
    }

    /* sliding window: v[0]=a(i-5), v[1]=a(i-4), …, v[4]=a(i-1) */
    seq_t v[5];
    for (idx_t i = 0; i < 5; ++i) {
        v[i] = init[i];
    }

    seq_t next = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        next = v[4] + (seq_t)1154 * v[3]
                  - (seq_t)1154 * v[2]
                  - v[1]
                  + v[0];

        /* shift window for next iteration */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return next;
}
