/*
 * A290927  p-INVERT of the positive integers, where p(S) = (1 - S^2)^3.
 * https://oeis.org/A290927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 63*a(n-2) + 196*a(n-3) - 414*a(n-4) +
 *            636*a(n-5) - 731*a(n-6) + 636*a(n-7) - 414*a(n-8) + 196*a(n-9) -
 *            63*a(n-10) + 12*a(n-11) - a(n-12) Initial values: a(0)=0,
 *            a(1)=3, a(2)=12, a(3)=36, a(4)=108, a(5)=331, a(6)=1008,
 *            a(7)=3027, a(8)=8992, a(9)=26502, a(10)=77592, a(11)=225806,...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 13 values (index 0..12) */
    static const seq_t init[13] = {
        (seq_t)0, (seq_t)3, (seq_t)12, (seq_t)36, (seq_t)108,
        (seq_t)331, (seq_t)1008, (seq_t)3027, (seq_t)8992,
        (seq_t)26502, (seq_t)77592, (seq_t)225806, (seq_t)653544
    };

    if (n < 13)
        return init[(size_t)n];

    /* buffer holding the last 12 terms: buf[0]=a(k-12), buf[11]=a(k-1) */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i + 1];              /* a(1) .. a(12) */

    /* signed coefficients for a(k-12) .. a(k-1) */
    static const int coeff[12] = {
        -1, 12, -63, 196, -414, 636, -731,
        636, -414, 196, -63, 12
    };

    idx_t cur = 13;                       /* current term to compute */

    while (cur <= n) {
        seq_t pos = 0, neg = 0;
        for (size_t i = 0; i < 12; ++i) {
            if (coeff[i] >= 0)
                pos += (seq_t)coeff[i] * buf[i];
            else
                neg += (seq_t)(-coeff[i]) * buf[i];
        }
        seq_t new_val = pos - neg;        /* unsigned subtraction; pos >= neg */

        /* shift buffer left by one position */
        for (size_t i = 0; i < 11; ++i)
            buf[i] = buf[i + 1];
        buf[11] = new_val;

        ++cur;
    }

    return buf[11];
}
