/*
 * A323262  The sequence denoted by a_n used in the calculation of A323260.
 * https://oeis.org/A323262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4) - a(n-5)
   for n ≥ 7, with initial values
   a(1)=1, a(2)=1, a(3)=5, a(4)=20, a(5)=83, a(6)=337.
   The recurrence follows from the generating function
   G.f.: x + x²(1‑2x²+3x³)/(1‑5x+3x²+5x³‑7x⁴+x⁵). */

seq_t a(idx_t n)
{
    /* initial 6 terms */
    static const seq_t init[7] = {0, 1, 1, 5, 20, 83, 337};

    if (n <= 6) {
        return init[n];
    }

    seq_t a1 = init[1];   /* a(n-5) */
    seq_t a2 = init[2];   /* a(n-4) */
    seq_t a3 = init[3];   /* a(n-3) */
    seq_t a4 = init[4];   /* a(n-2) */
    seq_t a5 = init[5];   /* a(n-1) */
    seq_t a6 = init[6];   /* a(n) at current step */

    idx_t i;
    for (i = 7; i <= n; ++i) {
        /* Compute next term using a n-1 ... a n-5 */
        seq_t tA = 5 * a6 + 7 * a3;               /* 5*a(n-1)+7*a(n-4) */
        seq_t tB = 3 * a5 + 5 * a4 + a2;          /* 3*a(n-2)+5*a(n-3)+a(n-5) */
        seq_t next = tA - tB;                     /* a(n+1) */

        /* Shift last five terms */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
