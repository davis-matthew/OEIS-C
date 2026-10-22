/*
 * A349330  a(n) = Sum_{d|n} d^c(d), where c is the characteristic function of
 *          squares (A010052).
 * https://oeis.org/A349330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d^c(d) where c(d)=1 iff d is a perfect square, 0
 *            otherwise. Implementation: factor n, generate all divisors
 *            recursively; a divisor is a square iff all its prime exponents
 *            are even. Sum all square divisors, count the non‑square
 *            divisors, and return the total
 * Range:     u8 n <= 224, u16 n <= 8203, u32 n <= 8203, u64 n <= 8203,
 *            u128 n <= 8203, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void dfs(const uint64_t *pr, const uint64_t *ex, int total, int pos,
                seq_t curr, bool square_flag,
                seq_t *sum_sq, uint64_t *cnt_non_sq)
{
    if (pos == total) {
        if (square_flag)
            *sum_sq = *sum_sq + curr;
        else
            (*cnt_non_sq)++;
        return;
    }

    uint64_t p = pr[pos];
    seq_t d = curr;
    for (unsigned e = 0; e <= (unsigned)ex[pos]; ++e) {
        bool next_square = square_flag && ((e & 1) == 0);
        dfs(pr, ex, total, pos + 1, d, next_square, sum_sq, cnt_non_sq);
        d = d * (seq_t)p;   /* multiply by p once for the next exponent */
    }
}

seq_t a(idx_t n)
{
    /* Handle trivial case n == 1 */
    if (n == 1) return (seq_t)1;

    /* Factor the integer n */
    uint64_t pr[64];   /* prime factors */
    uint64_t ex[64];   /* corresponding exponents */
    size_t  cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Recursively generate divisors */
    seq_t sum_sq = 0;
    uint64_t cnt_non_sq = 0;
    dfs(pr, ex, (int)cnt, 0, (seq_t)1, true, &sum_sq, &cnt_non_sq);

    return sum_sq + (seq_t)cnt_non_sq;
}
