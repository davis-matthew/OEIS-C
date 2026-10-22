/*
 * A363157  Expansion of Sum_{k>0} k * x^(4*k-1) / (1 - x^(5*k-2)).
 * https://oeis.org/A363157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{⌊(n+1)/4⌋} k if (n-(4k-1)) mod (5k-2) == 0, else
 *            0
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1, value for n=0 is undefined */

    seq_t sum = 0;
    idx_t max_k = (n + 1) / 4;      /* largest k with 4k-1 ≤ n */

    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t rem = n - (4 * k - 1);       /* n - (4k-1)  */
        idx_t div = 5 * k - 2;             /* 5k-2       */

        if (rem % div == 0) {
            sum += (seq_t)k;              /* add k to the sum   */
        }
    }
    return sum;
}
