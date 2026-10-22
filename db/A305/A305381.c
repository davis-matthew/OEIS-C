/*
 * A305381  Number of 1's in truth table for Boolean function x1 x2 x4 + x2 x3
 *          x5 + ... + x{n-3} x{n-2} xn + x{n-2} x{n-1} x1 + x{n-1} xn x2 + xn
 *          x1 x3.
 * https://oeis.org/A305381
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) + 4*a(n-5) - 8*a(n-6)
 *            Initial values (n ≥ 4): a(4) = 4, a(5) = 6, a(6) = 24, a(7) =
 *            36, a(8) = 112, a(9) = 184 The recurrence uses only integer
 *            arithmetic and the built‑in shift operators. For n < 4 the
 *            function returns 0 (undefined in the sequence)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* indices below the offset are irrelevant – return 0 */
    if (n < 4) return (seq_t)0;

    /* small static base table – valid in C for integer types and in C++ for
       arbitrary-precision types because the compiler provides the required
       constructor.  The first four entries are dummy to use 0‑based indexing. */
    static const seq_t base[10] = {
        0, 0, 0, 0,      /* a(0) … a(3) – unused */
        4, 6, 24, 36, 112, 184
    };

    /* For n less than 10 we can return the pre‑computed value. */
    if (n < 10) return base[n];

    /* Keep the last six terms needed for the recurrence.  The array indices
       represent the following values:
           v[0] = a(i-6)
           v[1] = a(i-5)
           v[2] = a(i-4)
           v[3] = a(i-3)
           v[4] = a(i-2)
           v[5] = a(i-1)   */
    seq_t v[6];
    v[0] = base[4]; v[1] = base[5]; v[2] = base[6];
    v[3] = base[7]; v[4] = base[8]; v[5] = base[9];

    /* Iterate from 10 up to n, updating the sliding window. */
    for (idx_t i = 10; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-2) - 4*a(i-3) + 4*a(i-5) - 8*a(i-6) */
        seq_t next =
            (v[5] << 1) +            /* 2*a(i-1) */
            (v[4] << 1) -            /* 2*a(i-2) */
            (v[3] << 2) +            /* -4*a(i-3) */
            (v[1] << 2) -            /* +4*a(i-5) */
            (v[0] << 3);             /* -8*a(i-6) */

        /* Shift the window to the left: drop a(i-6), add a(i). */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    /* After the loop, v[5] holds a(n). */
    return v[5];
}
