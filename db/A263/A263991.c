/*
 * A263991  a(n) is the number of uniform consecutive subintervals of the unit
 *          interval each of size 2^(-ceiling(log_2(n))) that are completely
 *          covered by one of the n uniform consecutive subintervals (of size
 *          1/n each) of the unit interval.
 * https://oeis.org/A263991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d * (floor((t - n)/d) + 1), where d = 2^(v2(n)), t =
 *            2^ceil(log2(n)). This counts big subintervals of length
 *            2^-ceil(log2(n)) that lie completely inside one of the n equal
 *            small subintervals
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, guard though offset is 1 */
    /* compute ceil(log2(n)) as k */
    idx_t k;
    if (n == 1) {
        k = 0;
    } else {
        k = 64 - __builtin_clzll(n - 1);   /* ceil(log2(n)) for n>1 */
    }

    /* t = 2^k */
    seq_t t = ((seq_t)1) << k;

    /* d = lowest power of two dividing n, i.e. n & -n */
    seq_t sn = (seq_t)n;
    seq_t d = sn & ((~sn) + 1);   /* two's complement negation */

    /* remaining length */
    seq_t t_minus_n = t - sn;     /* t >= n always */

    seq_t q = t_minus_n / d;      /* floor division */
    seq_t result = d * (q + 1);

    return result;
}
