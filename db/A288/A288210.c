/*
 * A288210  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binary string that encodes the diagonal of Rule 553. •
 *            For odd n: the string consists of n ones followed by a trailing
 *            zero. • For even n>0: let L = 2^{⌊log₂ n⌋ − 1} − 1 (the length
 *            of the zero block). Then the string has (n − L) ones, followed
 *            by L zeros, followed by a final one
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)                  /* single black cell at stage 0 */
        return (seq_t)1;

    seq_t res = (seq_t)0;
    const seq_t ten = (seq_t)10;
    const seq_t one = (seq_t)1;
    const seq_t zero = (seq_t)0;

    if (n & 1) {                 /* odd n */
        for (idx_t i = 0; i < n; ++i)
            res = res * ten + one;    /* append a 1 */
        res = res * ten + zero;       /* trailing 0 */
    }
    else {                       /* even n */
        /* Length of the zero block L */
        idx_t log2n = floor_log2_uint64(n);   /* log₂(n)   (n ≥ 2) */
        idx_t L = ((log2n == 0) ? 0 : ((idx_t)1 << (log2n - 1)) - 1);
        idx_t leading = n - L;           /* number of preceding 1’s */

        for (idx_t i = 0; i < leading; ++i)
            res = res * ten + one;        /* leading 1’s */
        for (idx_t i = 0; i < L; ++i)
            res = res * ten + zero;       /* zero block */
        res = res * ten + one;            /* final 1 */
    }
    return res;
}
