/*
 * A371768  a(n) = n! * [x^n] (log(2*x - 1) - log(x - 1) - log(x + 1)) / (x -
 *          1). Row sums of A371685.
 * https://oeis.org/A371768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] (log(2*x - 1) - log(x - 1) - log(x + 1)) / (x
 *            - 1) Simplified recurrence: a(0) = 0 for n ≥ 1: a(n) = n *
 *            a(n‑1) + (n‑1)! * 2^n if n is odd a(n) = n * a(n‑1) + (n‑1)! *
 *            (2^n – 2) if n is even
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) 
        return 0;

    seq_t fact = 1;     /* (n‑1)! for current n */
    seq_t pow2 = 1;     /* 2^0 */
    seq_t prev = 0;     /* a(n‑1) */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;                 /* now pow2 = 2^i */

        seq_t val;
        if (i & 1)                  /* n odd */
            val = pow2;
        else                        /* n even */
            val = pow2 - 2;

        seq_t term = fact * val;    /* (i‑1)! * val  */
        prev = (seq_t)i * prev + term; /* a(i) */

        fact *= (seq_t)i;           /* fact becomes i! for next iteration */
    }

    return prev;
}
