/*
 * A271947  a(n) = Product_{k=0..n} (7*k)!.
 * https://oeis.org/A271947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (7*k)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t fact = 1;
    seq_t result = 1;
    idx_t last = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = 7 * k;
        if (k > 0)
        {
            for (idx_t x = last + 1; x <= m; ++x)
            {
                fact = seq_mul(fact, (seq_t)x);
            }
            last = m;
        }
        result = seq_mul(result, fact);
    }
    return result;
}
