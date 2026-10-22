/*
 * A355826  Dirichlet inverse of A355825, characteristic function of
 *          exponentially odious numbers.
 * https://oeis.org/A355826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Dirichlet inverse of f(n)=A355825(n). The function is
 *            multiplicative; for n = p^k (p prime, k≥0) let h2[k] be a(p^k).
 *            Then h2[0] = 1 and for k>0: h2[k] = - Σ_{i=1..k} g(i) * h2[k-i],
 *            where g(i)=1 if popcount(i) is odd (i.e. A355825(p^i)=1) and
 *            g(i)=0 otherwise. The sequence a(n) is the pro...
 * Range:     i8 n <= 9297, i16 n <= 9297, i32 n <= 9297, i64 n <= 9297,
 *            i128 n <= 9297, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t h2[65];
static bool h2_init = false;

static void init_h2(void) {
    if (h2_init) return;
    h2_init = true;
    h2[0] = (seq_t)1;
    for (idx_t k = 1; k < 65; ++k) {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i <= k; ++i) {
            /* A355825(p^i)=1 iff popcount(i) is odd. */
            int pop = __builtin_popcountll((uint64_t)i);
            if (pop & 1) sum += h2[k - i];
        }
        h2[k] = -sum;
    }
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    init_h2();

    /* Factorise n.  The maximum number of distinct primes in a 64‑bit
       integer is <= 16, 20 is generous. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        idx_t e = (idx_t)exps[i];
        result *= h2[e];
    }
    return result;
}
