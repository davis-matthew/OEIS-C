/*
 * A301787  Number of nX5 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) for n >= 5 with a(1)=8, a(2)=14,
 *            a(3)=25, a(4)=45
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) without dynamic memory allocation, so it works
   for both C++ (arbitrary‑precision seq_t) and C (fixed‑size uint) */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)45;

    /* a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = (seq_t)8;   /* a(1)      */
    seq_t a2 = (seq_t)14;  /* a(2)      */
    seq_t a3 = (seq_t)25;  /* a(3)      */
    seq_t a4 = (seq_t)45;  /* a(4)      */

    /* Iterate from i = 5 up to n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_a = a4;
        new_a += a4;      /* 2 * a(n-1) */
        new_a -= a1;      /* subtract a(n-4) */

        /* Shift window: a1 <- a2, a2 <- a3, a3 <- a4, a4 <- new_a */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_a;
    }

    return a4;
}
