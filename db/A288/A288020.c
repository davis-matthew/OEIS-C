/*
 * A288020  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 501", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   OEIS A288020 a(0)=1, a(1)=10, a(2)=1, a(3)=1110, a(4)=11,
 *            a(5)=111110, For n>4: if n is even: a(n)=11 if n is odd (n>5):
 *            a(n)=10*(10^n - 10)/9 (holds for all odd n>5) This implements
 *            the sequence based on that recurrence
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include <stdint.h>

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)10;
        case 2: return (seq_t)1;
        case 3: return (seq_t)1110;
        case 4: return (seq_t)11;
    }
    if ((n & 1) == 0) {         /* even n > 4 */
        return (seq_t)11;
    }
    if (n == 5) {                /* special case */
        return (seq_t)111110;
    }
    /* odd n > 5 */
    seq_t t = pow10(n);          /* 10^n */
    t -= (seq_t)10;              /* 10^n - 10 */
    seq_t q = t / (seq_t)9;      /* (10^n - 10) / 9 */
    return q * (seq_t)10;        /* 10 * ((10^n - 10) / 9) */
}
