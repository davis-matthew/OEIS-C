/*
 * A339321  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^6.
 * https://oeis.org/A339321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of factorizations of n into factors > 1 of 6 kinds
 * Range:     u8 n <= 1, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom_group
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>
#include "../utils/binom_group.h"

/* recursive enumerator of unordered factorizations */
static seq_t dfs(idx_t n, idx_t min_factor)
{
    seq_t total = 0;

    /* gather all divisors of n that are >= min_factor */
    uint64_t divs[256];
    size_t div_cnt = 0;

    for (idx_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            divs[div_cnt++] = d;
            idx_t other = n / d;
            if (other != d && other >= min_factor) {
                divs[div_cnt++] = other;
            }
        }
    }
    if (n >= min_factor) {
        divs[div_cnt++] = n;   /* n itself is a divisor */
    }

    /* sort ascending – divisor count is tiny, simple insertion sort suffices */
    if (div_cnt > 1) {
        for (size_t i = 1; i < div_cnt; ++i) {
            uint64_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
    }

    /* iterate over each divisor as the first factor of a group */
    for (size_t idx = 0; idx < div_cnt; ++idx) {
        idx_t d = divs[idx];
        if (d < min_factor) continue;   /* safety */

        idx_t pow = d;
        idx_t m = 1;   /* multiplicity of this factor */

        while (pow <= n && n % pow == 0) {
            seq_t group_val = binom_group(m);
            idx_t rem = n / pow;

            if (rem == 1) {
                total += group_val;            /* finished factorization */
            } else {
                total += group_val * dfs(rem, d + 1);
            }

            /* prepare next multiplicity */
            if (pow > n / d) break;   /* would overflow or exceed n */
            pow *= d;
            ++m;
        }
    }

    return total;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* offset 1 */
    return dfs(n, 2);              /* all factors must be >= 2 */
}
