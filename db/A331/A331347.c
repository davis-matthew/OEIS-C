/*
 * A331347  Number of permutations w in S_n that form Boolean intervals [s, w]
 *          in the Bruhat order for every simple reflection s in the support
 *          of w.
 * https://oeis.org/A331347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fib(2*n - 1) + n - 2 (for n ≥ 2), with a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe for completeness */
    if (n == 1) return 1;          /* special case, see comment above */

    /* Compute fib(2*n - 1). 2*n fits in idx_t safely for our n range. */
    idx_t m = 2 * n - 1;
    seq_t value = fib(m);

    /* Add n – 2, cast n to seq_t first to avoid truncation. */
    value += (seq_t)n - 2;
    return value;
}
