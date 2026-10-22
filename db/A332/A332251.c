/*
 * A332251  a(n) is the real part of f(n) defined by f(0) = 0 and f(n+1) =
 *          f(n) + i^A000120(n) (where i denotes the imaginary unit). Sequence
 *          A332252 gives imaginary parts.
 * https://oeis.org/A332251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = sum_{k=0}^{n-1} c(k) where * c(k) = 1 if popcount(k) ≡ 0
 *            (mod 4) * -1 if popcount(k) ≡ 2 (mod 4) * 0 otherwise *
 *            Equivalently, a(n) is the real part of the complex walk *
 *            f(0)=0, f(n+1)=f(n)+i^{popcount(n)}, where
 *            i^0=1,i^1=i,i^2=-1,i^3=-i. * The implementation follows the
 *            binary recursion...
 * Range:     i8 n <= 8192, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_part = 0, b_part = 0;   /* real and imaginary components */
    if (n == 0)                        /* a(0)=0 */
        return 0;

    int popcnt = 0;                    /* popcount of the prefix processed so far */

    /* find most significant bit of n (n>0) */
    int msb = 63 - __builtin_clzll((unsigned long long)n);

    for (int i = msb; i >= 0; --i) {
        /* double: f := f * (1 + i)  =>  real' = real - imag, imag' = real + imag */
        seq_t a_twice = a_part - b_part;
        seq_t b_twice = a_part + b_part;
        a_part = a_twice;
        b_part = b_twice;

        /* if the current bit is 1 we add i^{popcount(previous bits)} */
        if ((n >> i) & 1) {
            int mod4 = popcnt & 3;
            switch (mod4) {
            case 0: /* i^0 = 1 */
                a_part += (seq_t)1;
                break;
            case 2: /* i^2 = -1 */
                a_part += (seq_t)-1;
                break;
            case 1: /* i^1 = i */
                b_part += (seq_t)1;
                break;
            case 3: /* i^3 = -i */
                b_part += (seq_t)-1;
                break;
            }
            ++popcnt;                 /* one more 1 bit has been seen */
        }
    }
    return a_part;                     /* real part of f(n) */
}
