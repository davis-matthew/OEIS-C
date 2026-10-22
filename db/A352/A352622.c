/*
 * A352622  Number of regular convex polytopes that can be formed with n
 *          indistinguishable points located at the vertices, coinciding in
 *          equal frequency at each vertex, if coinciding at all.
 * https://oeis.org/A352622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A111336(d)
 * Range:     u8 n <= 1801, u16 n <= 1801, u32 n <= 1801, u64 n <= 1801,
 *            u128 n <= 1801, big any n
 * Helpers:   _a111336, _is_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A111336(d) counts distinct regular convex polytopes with d vertices. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_a111336.h"
#include "../utils/_is_pow2.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;   /* undefined offset, but safe to return 0 */

    seq_t sum = 0;
    for (idx_t i = 1u; i * i <= n; ++i)
    {
        if (n % i == 0u)
        {
            sum += _a111336(i);
            idx_t j = n / i;
            if (j != i)
                sum += _a111336(j);
        }
    }
    return sum;
}
