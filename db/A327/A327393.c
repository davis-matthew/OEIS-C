/*
 * A327393  Maximum stable divisor of n.
 * https://oeis.org/A327393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max{d | d divides n and d is stable}, where a stable
 *            number has distinct prime indices that are pairwise indivisible
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"      /* factor_u64(n, pr, ex, &cnt) */
#include "../utils/compute_pi.h"     /* compute_pi(p) -> idx of prime p */
#include "../utils/pow_int.h"        /* pow_int(base, exp) returns seq_t */

/* Recursive search of stable prime subsets */
static void dfs(size_t pos, seq_t prod,
                const idx_t *idxs, const seq_t *pow_arr, size_t r,
                size_t *chosenCnt, idx_t *chosen, seq_t *best)
{
    for (size_t i = pos; i < r; ++i) {
        idx_t newIdx = idxs[i];
        bool ok = true;
        for (size_t j = 0; j < *chosenCnt; ++j) {
            if (newIdx % chosen[j] == 0 || chosen[j] % newIdx == 0) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        seq_t newProd = prod * pow_arr[i];
        if (newProd > *best) *best = newProd;

        chosen[(*chosenCnt)++] = newIdx;
        dfs(i + 1, newProd, idxs, pow_arr, r, chosenCnt, chosen, best);
        --(*chosenCnt);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n == 1 is trivial */
    if (n <= 1) return (seq_t)1;

    /* Factor n.  64 distinct primes are enough for any 64‑bit number. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Prepare arrays with prime indices and full prime‑power values. */
    idx_t   idxs[64];
    seq_t   pow_arr[64];
    for (size_t i = 0; i < cnt; ++i) {
        idxs[i]   = (idx_t)compute_pi(pr[i]);          /* index of prime pr[i] */
        pow_arr[i] = pow_int((seq_t)pr[i], (idx_t)ex[i]); /* p^e */
    }

    seq_t best = (seq_t)1;
    size_t chosenCnt = 0;
    idx_t   chosen[64];

    dfs(0, (seq_t)1, idxs, pow_arr, cnt, &chosenCnt, chosen, &best);

    return best;
}
