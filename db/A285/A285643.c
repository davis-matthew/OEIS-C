/*
 * A285643  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 65", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = • for n = 0, 2, 4 : 1 • for n = 1 : 10 • for n = 3 : 1100
 *            • for n > 4 and even : 11 • for n > 4 and odd : 100 * ((10^(n-1)
 *            - 1) / 9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility pow10(e) which returns 10^e as a seq_t. */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1100;
    if (n == 4) return (seq_t)1;
    if ((n & 1) == 0) return (seq_t)11;   /* even n > 4 */

    /* odd n > 4 */
    idx_t e = n - 1;                     /* exponent for 10^(n-1) */
    seq_t ten_pow = pow10(e);            /* 10^(n-1) */
    seq_t tmp = ten_pow - (seq_t)1;      /* 10^(n-1) - 1 */
    seq_t div9 = tmp / (seq_t)9;         /* (10^(n-1)-1)/9  */
    seq_t res = div9 * (seq_t)100;       /* multiply by 100 to append two zeros */
    return res;
}
