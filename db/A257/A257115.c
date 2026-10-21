/*
 * A257115  Smallest k such that none of k + 1, k + 3,... k + 2n - 1 are
 *          squarefree and all of k + 2, k + 4,... k + 2n are squarefree.
 * https://oeis.org/A257115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest k such that none of k+1, k+3, ... k+2n-1 are
 *            squarefree and all of k+2, k+4, ... k+2n are squarefree
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Cache results for n < 9 (the only defined values). */
    static seq_t cache[9];
    static bool computed[9] = { false };

    if (n < 9) {
        if (computed[n]) {
            return cache[n];
        }

        /* n == 0: the smallest positive k is 1 by convention. */
        if (n == 0) {
            cache[0] = (seq_t)1;
            computed[0] = true;
            return cache[0];
        }

        seq_t k = 1;
        while (1) {
            bool ok = true;

            /* Check odd offsets: k + 1, k + 3, ..., k + 2n - 1
               must NOT be squarefree. */
            for (idx_t j = 0; j < n; ++j) {
                seq_t val = k + (seq_t)(2 * j + 1);
                if (is_squarefree(val)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                ++k;
                continue;
            }

            /* Check even offsets: k + 2, k + 4, ..., k + 2n
               must be squarefree. */
            for (idx_t j = 1; j <= n; ++j) {
                seq_t val = k + (seq_t)(2 * j);
                if (!is_squarefree(val)) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                cache[n] = k;
                computed[n] = true;
                return k;
            }
            ++k;
        }
    }

    /* For n >= 9 the sequence is undefined; return 0. */
    return (seq_t)0;
}
