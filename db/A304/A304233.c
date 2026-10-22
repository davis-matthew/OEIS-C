/*
 * A304233  If n = Product (p_j^k_j) then a(n) = min{p_j^k_j}*max{p_j^k_j}.
 * https://oeis.org/A304233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{p^k} * max{p^k} where n = ∏ p^k (prime power
 *            factorisation)
 * Range:     u8 n <= 15, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n <= 1u)
        return 1;

    uint64_t nn = (uint64_t)n;
    size_t cnt;
    uint64_t pr[32], ex[32];

    /* Factor n into prime powers */
    factor_u64(nn, pr, ex, &cnt);

    seq_t min_val = 0, max_val = 0;
    bool first = true;

    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^k */
        seq_t component = seq_pow((seq_t)pr[i], (idx_t)ex[i]);

        if (first) {
            min_val = component;
            max_val = component;
            first = false;
        } else {
            if (component < min_val)
                min_val = component;
            if (component > max_val)
                max_val = component;
        }
    }

    /* Return the product of the smallest and largest prime power components */
    return min_val * max_val;
}
