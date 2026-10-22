/*
 * A340853  Number of factorizations of n such that every factor is a multiple
 *          of the number of factors.
 * https://oeis.org/A340853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into integers > 1
 *            such that each factor is a multiple of the number of factors
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t dfs(idx_t rem, idx_t start, int depth, seq_t *factors)
{
    seq_t cnt = 0;
    for (idx_t f = start; f <= rem / f; ++f) {
        if (rem % f == 0) {
            factors[depth] = (seq_t)f;
            cnt += dfs(rem / f, f, depth + 1, factors);
        }
    }
    if (rem >= start) {
        factors[depth] = (seq_t)rem;
        int k = depth + 1;
        bool ok = true;
        for (int i = 0; i < k; ++i) {
            if (factors[i] % (seq_t)k != 0) {
                ok = false;
                break;
            }
        }
        if (ok) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t factors[20];
    return dfs(n, 2, 0, factors);
}
