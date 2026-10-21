/*
 * A265054  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 63, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=1, a(1)=4, a(2)=10, a(3)=22, a(4)=46, a(5)=94,
   a(6)=188, a(7)=372, a(8)=734, a(9)=1446.
   For n ≥ 10:
   a(n) = 3*a(n‑1) – 2*a(n‑2) – a(n‑3) + 3*a(n‑4) – 3*a(n‑5) + a(n‑6)
          + 2*a(n‑7) – 3*a(n‑8) + a(n‑9). */

seq_t a(idx_t n)
{
    /* initial terms a(0) … a(9) */
    static const seq_t init[10] = {
        (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)22,
        (seq_t)46, (seq_t)94, (seq_t)188, (seq_t)372,
        (seq_t)734, (seq_t)1446
    };

    if (n < 10)
        return init[n];

    /* buffer holds the last 9 terms:
       buffer[0] = a_{current-9}, … , buffer[8] = a_{current-1} */
    seq_t buffer[9];
    for (idx_t j = 0; j < 9; ++j)
        buffer[j] = init[j + 1];   /* a1 … a9 */

    /* compute successive terms until a(n) */
    for (idx_t k = 10; k <= n; ++k) {
        seq_t next =
            (seq_t)3 * buffer[8]   - (seq_t)2 * buffer[7]
          - buffer[6]               + (seq_t)3 * buffer[5]
          - (seq_t)3 * buffer[4]    + buffer[3]
          + (seq_t)2 * buffer[2]   - (seq_t)3 * buffer[1]
          + buffer[0];
        /* shift buffer left and append next */
        for (idx_t j = 0; j < 8; ++j)
            buffer[j] = buffer[j + 1];
        buffer[8] = next;
    }
    return buffer[8];   /* now buffer[8] contains a(n) */
}
