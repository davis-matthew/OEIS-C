/*
 * A287767  Brazilian numbers whose repdigits are the same as the repdigits of
 *          the base.
 * https://oeis.org/A287767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the value of a repdigit in base * b = d * (10^m - 1) / 9
 *            where the base itself is a * repdigit of length m with digit d.
 *            The number is then * d * (b^m - 1) / (b - 1). The enumeration of
 *            (m,d) * follows the OEIS ordering: * m = 2, d = 2,3,…,9 (8
 *            terms) * m = 3, d = 1,2,…,9 (9 terms) * m =...
 * Range:     u8 none, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t m, d;
    if (n <= (idx_t)8) {          /* 8 terms with m = 2 and d = 2..9 */
        m = 2;
        d = n + 1;                 /* d runs from 2 to 9 */
    } else {
        idx_t k = n - (idx_t)8;    /* remaining index after first 8 terms */
        m = 3 + (k - 1) / 9;       /* m increases every 9 terms, starting at 3 */
        d = ((k - 1) % 9) + 1;     /* d cycles 1..9 */
    }

    /* base b = d * (10^m - 1) / 9 */
    seq_t pow10m = big_pow((seq_t)10, (seq_t)m);
    seq_t base = ((pow10m - (seq_t)1) / (seq_t)9) * (seq_t)d;

    /* b^m, then (b^m - 1) / (b - 1) * d */
    seq_t pow_b_m = big_pow(base, (seq_t)m);
    seq_t numerator   = pow_b_m - (seq_t)1;
    seq_t denominator = base - (seq_t)1;
    seq_t val = (numerator / denominator) * (seq_t)d;

    return val;
}
