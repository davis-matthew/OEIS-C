/*
 * A305913  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+a(n-2)+7*a(n-3)-19*a(n-4)-21*a(n-5)+18*a(n-6)
 *            -23*a(n-7)+17*a(n-8)+55*a(n-9)+83*a(n-10)-23*a(n-11)
 *            -120*a(n-12)-20*a(n-13)+78*a(n-14)+9*a(n-15)-22*a(n-16)
 *            +0*a(n-17)+2*a(n-18) for n > 20 Initial values: a(1)=3, a(2)=9,
 *            a(3)=13, a(4)=27, a(5)=62, a(6)=135, a(7)=312, a(8)=825,
 *            a(9)=...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* initial 20 terms of the sequence */
static const seq_t INIT[20] = {
    (seq_t)3,   (seq_t)9,   (seq_t)13,  (seq_t)27,   (seq_t)62,
    (seq_t)135, (seq_t)312, (seq_t)825, (seq_t)2367, (seq_t)6940,
    (seq_t)21330,(seq_t)67814,(seq_t)216690,(seq_t)699884,
    (seq_t)2279373,(seq_t)7427118,(seq_t)24244543,(seq_t)79292187,
    (seq_t)259332414,(seq_t)848386032
};

/* coefficients for the recurrence, for k = 1 .. 18  (a(n-k)) */
static const int16_t COEFFS[18] = {
    3,  1,  7, -19, -21, 18, -23, 17,
    55, 83, -23, -120, -20, 78, 9, -22,
    0,  2
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n <= 20) return INIT[n-1];

    /* buffer holding the last 18 computed terms (a(i-1)..a(i-18)) */
    seq_t buf[18];
    for (int j = 0; j < 18; ++j)
        buf[j] = INIT[19 - j];               /* buf[0] = a(20), buf[17] = a(3) */

    seq_t current = 0;

    for (idx_t i = 21; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 18; ++k) {
            idx_t idx = i - k;               /* index of the term a(idx) */
            const seq_t term = (idx <= 20) ? INIT[idx-1] : buf[k-1];
            const int coeff = COEFFS[k-1];
            if (coeff > 0)
                pos += term * (seq_t)coeff;
            else
                neg += term * (seq_t)(-coeff);
        }
        current = pos - neg;                 /* pos >= neg always */

        if (i != n) {                       /* prepare buffer for next step */
            for (int j = 17; j > 0; --j)
                buf[j] = buf[j-1];
            buf[0] = current;
        }
    }
    return current;
}
