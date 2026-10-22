/*
 * A328966  Number of strict factorizations of n with integer average.
 * https://oeis.org/A328966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts all strictly increasing factorizations of n (into integers > 1)
 * whose arithmetic mean is an integer.  Each factorization is a multiset of
 * factors f1 < f2 < … < fk with f1 · f2 · … · fk = n; it is counted
 * iff (f1 + f2 + … + fk) % k == 0.
 *
 * The implementation explores the factorization tree recursively.
 * For each step it chooses the next factor d (>= min_factor) and
 * recurses on the remaining product n/d, ensuring the strict
 * increasing order by passing min_factor = d + 1.  When the remaining
 * product is 1 we have a complete factorization and the average
 * condition is checked.
 *
 * Complexity is modest for n ≤ 10 000 because for each call only the
 * divisors of the current product are considered, and the recursion
 * depth is ≤ log₂(n).
 */

#include <stdbool.h>

static seq_t dfs(idx_t n, idx_t min_factor, idx_t sum, idx_t cnt)
{
    seq_t total = 0;
    for (idx_t d = min_factor; d <= n; ++d) {
        if (n % d != 0) continue;          /* d is not a divisor of n */
        idx_t rest = n / d;

        if (rest == 1) {                     /* final factor */
            if ((sum + d) % (cnt + 1) == 0) {
                total += 1;
            }
        } else if (rest >= d + 1) {          /* next factor must be strictly greater */
            total += dfs(rest, d + 1, sum + d, cnt + 1);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* sequence is defined for n ≥ 2 only */
    return dfs(n, 2, 0, 0);
}
