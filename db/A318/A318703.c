/*
 * A318703  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          f(n) = Sum_{k=0..w} b_k * i^k * 2^floor(k/2) (where i denotes the
 *          imaginary unit); a(n) is the imaginary part of f(n).
 * https://oeis.org/A318703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 9224, i16 n <= 9224, i32 n <= 9224, i64 n <= 9224,
 *            i128 n <= 9224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318703
 *  Formula:
 *    a(n) = Σ_{k: the k‑th bit of n is 1}
 *           [ (k mod 4 == 1)  ?  2^{⌊k/2⌋}
 *             : (k mod 4 == 3) ? -2^{⌊k/2⌋}
 *             : 0 ]
 *  Recurrence (used for reference):
 *    a(2n)   = a(2n + 1)          for n ≥ 0
 *    a(4k)   = -2 * a(k)          for k ≥ 0
 *
 *  The code directly evaluates the sum; complexity is O(log n).
 */

seq_t a(idx_t n)
{
    seq_t imag = 0;
    seq_t pow  = 1;   /* 2^{⌊k/2⌋} for the current bit position k */
    idx_t  k    = 0;
    idx_t  m    = n;

    while (m) {
        if (m & 1) {
            idx_t mod4 = k & 3;
            if (mod4 == 1)
                imag += pow;      /* i^k = i → + */
            else if (mod4 == 3)
                imag -= pow;      /* i^k = -i → - */
        }
        m >>= 1;          /* move to the next bit */
        ++k;

        /* Every two bits the exponent ⌊k/2⌋ increments by 1,
           so we double pow after processing an even k (excluding k==0). */
        if ((k & 1) == 0)  /* k even and > 0 */
            pow *= 2;
    }

    return imag;
}
