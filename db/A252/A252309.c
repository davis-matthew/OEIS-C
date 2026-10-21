/*
 * A252309  Number of (4+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 34, u64 n <= 82, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A252309
   Recurrence: a(n) = 7*a(n-3) - 14*a(n-6) + 8*a(n-9)   for n > 15
   Initial values (n = 1 … 15):
   1763, 1516, 2151, 3032, 5230,
   8130, 11639, 20350, 31954, 45815,
   80430, 126682, 181895, 319918, 504586
   This implementation computes a(n) for any n ≥ 1,
   using an O(n) forward loop that holds only the last nine
   computed values necessary for the recurrence. */
seq_t a(idx_t n)
{
    /* First 15 values (required for the recurrence). */
    static const seq_t init[15] = {
        1763, 1516, 2151, 3032, 5230,
        8130, 11639, 20350, 31954, 45815,
        80430, 126682, 181895, 319918, 504586
    };

    if (n <= 15)                /* directly return the known values */
        return init[n - 1];

    /* The recurrence needs the last nine values:
       buf[0] = a(i‑9), buf[1] = a(i‑8), …, buf[8] = a(i).  */
    seq_t buf[9];
    for (int j = 0; j < 9; ++j)
        buf[j] = init[6 + j];   /* starts with a(7)…a(15) */

    for (idx_t i = 16; i <= n; ++i) {
        /* Compute a(i) = 7*a(i‑3) - 14*a(i‑6) + 8*a(i‑9).  */
        seq_t term3 = buf[6];   /* a(i‑3)   */
        seq_t term6 = buf[3];   /* a(i‑6)   */
        seq_t term9 = buf[0];   /* a(i‑9)   */
        seq_t val = 7 * term3 + 8 * term9 - 14 * term6;

        /* Shift the buffer left: a(i‑8)…a(i) becomes a((i+1)‑9)…a(i+1). */
        for (int j = 0; j < 8; ++j)
            buf[j] = buf[j + 1];
        buf[8] = val;           /* a(i) is now the newest value */
    }

    return buf[8];                /* a(n) is stored at buf[8] after the loop */
}
