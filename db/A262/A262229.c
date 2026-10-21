/*
 * A262229  Numbers m such that A000217(m) > A062918(m).
 * https://oeis.org/A262229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that * sum_{k=1}^m k > sum_{k=1}^m
 *            rev_decimal(k), * where rev_decimal(k) is the decimal digit
 *            reversal of k
 * Range:     u8 n <= 18, u16 n <= 1274, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   _rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_rev_decimal.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; offset starts from 1 */

    idx_t  m      = 0;     /* current integer */
    idx_t  count  = 0;     /* number of terms found */
    long long diff = 0;    /* S_m - T_m, may be negative */

    while (count < n) {
        ++m;
        seq_t rev = _rev_decimal((seq_t)m);
        diff += (long long) m - (long long) rev;
        if (diff > 0) ++count;
    }
    return (seq_t) m;
}
