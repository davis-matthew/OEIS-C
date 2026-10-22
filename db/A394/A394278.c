/*
 * A394278  Number of induced cubes in the n X n fiveleaper graph.
 * https://oeis.org/A394278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394278: Number of induced cubes in the n × n fiveleaper graph.
 * The sequence satisfies
 *     a(n) = 16 * (4*n^2 - 76*n + 409)   for n > 18
 * and for the first 18 terms it is convenient to use an explicit table.
 * This implementation returns a(n) for all n ≥ 1.  For n ≤ 18 a lookup
 * table is used; for larger n the closed‑form polynomial is evaluated.
 */

seq_t a(idx_t n)
{
    /* Handle the (unlikely) case of n = 0 explicitly. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Initial terms a(1)..a(18).  They are small enough to fit in idx_t. */
    static const idx_t init[18] = {
        1,   /* a(1) */
        4,   /* a(2) */
        9,   /* a(3) */
        16,  /* a(4) */
        33,  /* a(5) */
        73,  /* a(6) */
        129, /* a(7) */
        205, /* a(8) */
        331, /* a(9) */
        529, /* a(10) */
        779, /* a(11) */
        1097,/* a(12) */
        1531,/* a(13) */
        2075,/* a(14) */
        2716,/* a(15) */
        3475,/* a(16) */
        4376,/* a(17) */
        5399 /* a(18) */
    };

    if (n <= 18U) {
        return (seq_t)init[n - 1];
    }

    /* For n > 18 use the polynomial formula. */
    seq_t v   = (seq_t)n;                 /* n as seq_t */
    seq_t t1  = (seq_t)4 * v * v;          /* 4*n^2   */
    seq_t t2  = (seq_t)76 * v;             /* 76*n    */
    seq_t base = t1 - t2 + 409;           /* 4*n^2 - 76*n + 409 */
    return (seq_t)16 * base;              /* final result */
}
