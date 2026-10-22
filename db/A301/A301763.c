/*
 * A301763  Number of ways to choose a constant rooted partition of each part
 *          in a constant rooted partition of n.
 * https://oeis.org/A301763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | (n-1)} τ(d-1)^{(n-1)/d} where τ(m) is the number
 *            of divisors of m
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 96, u64 n <= 192,
 *            u128 n <= 384, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_seq.h"

#define DIVISOR_LIMIT 10000

static idx_t tau[DIVISOR_LIMIT + 1];
static bool tau_initialized = false;

static void init_tau(void)
{
    if (tau_initialized) return;
    for (idx_t i = 1; i <= DIVISOR_LIMIT; ++i) {
        for (idx_t j = i; j <= DIVISOR_LIMIT; j += i) {
            tau[j] += 1;
        }
    }
    tau[0] = 1;         /* τ(0) = 1 by definition */
    tau_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; undefined below */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    init_tau();

    idx_t m = n - 1;
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d != 0) continue;

        /* first divisor d */
        seq_t base = tau[d - 1];          /* τ(d-1) */
        seq_t term = pow_seq(base, (int)(m / d));
        sum += term;

        /* paired divisor m/d if distinct */
        idx_t d2 = m / d;
        if (d2 != d) {
            seq_t base2 = tau[d2 - 1];
            seq_t term2 = pow_seq(base2, (int)(m / d2));
            sum += term2;
        }
    }
    return sum;
}
