/*
 * A296627  a(n) = BarnesG(4*n).
 * https://oeis.org/A296627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = BarnesG(4*n) (offset 0, a(0)=0)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Barnes G‑function satisfies G(1)=1 and
   G(m) = (m-2)! * G(m-1) for m ≥ 2.               */
/* This implementation computes G(m) directly, using
   only integer arithmetic.                      */
seq_t a(idx_t n)
{
    /* Return a(0) = 0 as per the OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* Compute G(4*n).  Let m = 4*n.  Use 64‑bit arithmetic for indices. */
    idx_t m = 4 * n;
    seq_t G = (seq_t)1; /* G(1) */
    seq_t f = (seq_t)1; /* f = (current i-2)! */

    for (idx_t i = 2; i <= m; ++i) {
        G *= f;                  /* G(i) = f * G(i-1)   */
        f *= (seq_t)(i - 1);     /* f updates to (i-1)! for next i  */
    }

    return G;
}
