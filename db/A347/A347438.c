/*
 * A347438  Number of unordered factorizations of n with alternating product
 *          1.
 * https://oeis.org/A347438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into integers >1
 *            * (weakly increasing) for which the alternating sum of the
 *            factors is 0. * Equivalently, for a factorization f1*f2*…*fk =
 *            n, * f1 - f2 + f3 - … (± fk) = 0 ⇔ (∏odd positions) = (∏even
 *            positions)
 * Range:     u8 n <= 9848, u16 n <= 9848, u32 n <= 9848, u64 n <= 9848,
 *            u128 n <= 9848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper: enumerates all unordered factorizations of remaining 'n'
 * with all chosen factors ≥ 'min_factor'.
 * 'depth' is the number of factors already stored in 'factors'.
 * 'cnt' accumulates the number of factorizations satisfying the alternating‑sum property. */
static void rec(idx_t n, idx_t min_factor, int depth, idx_t *factors, seq_t *cnt)
{
    /* Try every divisor d >= min_factor of the current n. */
    for (idx_t d = min_factor; d <= n; ++d) {
        if (n % d != 0) continue;               /* not a divisor */

        factors[depth] = d;

        if (n / d == 1) {                        /* complete factorization */
            /* evaluate alternating sum */
            long long diff = 0;
            for (int i = 0; i <= depth; ++i) {
                if ((i & 1) == 0) diff += (long long)factors[i];
                else              diff -= (long long)factors[i];
            }
            if (diff == 0) (*cnt)++;
        } else {
            /* recurse with the remaining part n/d, enforcing non‑decreasing order */
            rec(n / d, d, depth + 1, factors, cnt);
        }
    }
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;                          /* definition of a(1) */

    /* Maximum depth needed is at most the number of prime factors of n,
     * which is bounded by log2(n). 64 is large enough for any 64‑bit n. */
    idx_t factors[64];
    seq_t cnt = 0;

    rec(n, 2, 0, factors, &cnt);                  /* start with factor >1 */
    return cnt;
}
