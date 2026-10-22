/*
 * A306334  a(n) is the number of different linear hydrocarbon molecules with
 *          n carbon atoms.
 * https://oeis.org/A306334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 5*a(n-3) - a(n-4) - 2*a(n-6) +
 *            3*a(n-7) + a(n-8) - a(n-9), with a(1)=1, a(2)=3, a(3)=4,
 *            a(4)=10, a(5)=18, a(6)=42, a(7)=84, a(8)=192, a(9)=409.
 *            Undefined for n<1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Edge case: the OEIS offset is 1.  Returns 0 for 0. */
    if (n == 0)
        return (seq_t)0;

    /* The first nine terms – keep them static to avoid recomputation. */
    static const seq_t base[9] = {
        1, 3, 4, 10, 18, 42, 84, 192, 409
    };

    if (n <= 9)
        return base[n - 1];

    /* Use a sliding window of the last nine values.  This keeps memory
       usage constant and works for both integral and arbitrary‑precision
       integer types. */
    seq_t window[9];
    for (int k = 0; k < 9; ++k)
        window[k] = base[k];

    /* Compute successive terms up to n. */
    for (idx_t cur = 10; cur <= n; ++cur) {
        seq_t val =
            2 * window[8]   /* 2*a(cur-1) */
          + 3 * window[7]   /* +3*a(cur-2) */
          - 5 * window[6]   /* -5*a(cur-3) */
          -     window[5]   /* -a(cur-4)  */
          - 2 * window[3]   /* -2*a(cur-6)*/
          + 3 * window[2]   /* +3*a(cur-7)*/
          +     window[1]   /* +a(cur-8)  */
          -     window[0];  /* -a(cur-9)  */

        /* Shift the window left by one position. */
        for (int k = 0; k < 8; ++k)
            window[k] = window[k + 1];
        window[8] = val;
    }

    return window[8];
}
