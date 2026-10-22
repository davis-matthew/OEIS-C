/*
 * A315308  Coordination sequence Gal.5.299.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + offset offset = {1 if n==0 1 + 2*floor((n-1)/18) if
 *            n is odd (n>0) 0 if n is even and n<10 2 + 2*floor((n-10)/18) if
 *            n is even and n>=10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {                     /* special case  */
        return (seq_t)1;
    }

    const seq_t five = 5;
    seq_t  base = (seq_t)n * five;    /* 5·n */

    if (n & 1U) {                     /* odd n   */
        seq_t k = ((seq_t)n - 1) / 18;    /* floor((n-1)/18)  */
        seq_t offset = 1 + (k << 1);      /* 1 + 2*k       */
        return base + offset;
    } else {                          /* even n  */
        if (n < 10) {                  /* offset = 0   */
            return base;
        }
        seq_t k = ((seq_t)n - 10) / 18;    /* floor((n-10)/18)  */
        seq_t offset = 2 + (k << 1);       /* 2 + 2*k       */
        return base + offset;
    }
}
