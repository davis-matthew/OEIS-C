/*
 * A325045  Number of factorizations of n whose conjugate as an integer
 *          partition has no ones.
 * https://oeis.org/A325045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9954, u16 n <= 9954, u32 n <= 9954, u64 n <= 9954,
 *            u128 n <= 9954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of unordered factorizations of n (into factors ≥ 2)
 *               where the two largest factors are equal.
 *
 * The function is implemented recursively. We generate all multiplicative
 * partitions in non‑decreasing order and count only those whose last two
 * factors are equal. A special case handles n = 1, for which a(1) = 1.
 */
#include <stdint.h>

static void dfs_factor(uint64_t n, uint64_t min_factor, uint64_t depth,
                       uint64_t *factors, seq_t *cnt)
{
    /* Try to finish the factorization with 'n' as the last factor. */
    if (depth >= 1 && n == factors[depth - 1]) {
        *cnt = *cnt + (seq_t)1;
    }

    /* Continue exploring further factors in non‑decreasing order. */
    for (uint64_t f = min_factor; f <= n / f; ++f) {
        if (n % f == 0) {
            factors[depth] = f;
            dfs_factor(n / f, f, depth + 1, factors, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* by definition of the sequence */

    static uint64_t factors[64];          /* depth never exceeds 14 for n ≤ 10000 */
    seq_t count = (seq_t)0;

    dfs_factor((uint64_t)n, (uint64_t)2, (uint64_t)0, factors, &count);
    return count;
}
