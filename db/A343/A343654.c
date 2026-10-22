/*
 * A343654  Number of pairwise coprime sets of divisors > 1 of n.
 * https://oeis.org/A343654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S⊆P(n)} B_{|S|} · ∏_{p∈S} e_p where P(n) is the set of
 *            distinct prime factors of n, e_p is the exponent of p in n and
 *            B_k is the k‑th Bell number
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   binom, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    size_t m = cnt;
    seq_t sym[65] = {0};
    sym[0] = (seq_t)1;
    for (size_t i = 0; i < m; ++i) {
        seq_t e = (seq_t)exps[i];
        for (int j = (int)i + 1; j > 0; --j) {
            sym[j] += sym[j - 1] * e;
        }
    }

    seq_t bell[65] = {0};
    bell[0] = (seq_t)1;
    for (size_t i = 1; i <= m; ++i) {
        bell[i] = 0;
        for (size_t k = 0; k < i; ++k) {
            seq_t comb = binom((idx_t)i - 1, k);
            bell[i] += comb * bell[k];
        }
    }

    seq_t ans = 0;
    for (size_t k = 0; k <= m; ++k) {
        ans += sym[k] * bell[k];
    }
    return ans;
}
