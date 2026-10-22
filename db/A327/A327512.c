/*
 * A327512  Maximum divisor of n that is 1, 2, or a nonprime number whose
 *          prime indices are pairwise coprime.
 * https://oeis.org/A327512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd_int_simple, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the greatest divisor of n among
   1, 2, or a composite number whose prime indices
   (a prime m occurs as many times as its power in the
   divisor) are pairwise coprime. */

/* The algorithm factors n, then enumerates all allowed
   divisors by:
   - iterating over all powers of 2 that divide n,
   - for each odd prime factor p of n, choosing either
     0 or 1 occurrence (exponents >1 are forbidden because
     the same index would repeat),
   - ensuring that the chosen indices are pairwise coprime.
   The maximum divisor found is returned. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"
#include "../utils/gcd_int_simple.h"

/* recursive enumeration of allowed divisors */
static void dfs(const uint64_t *odd_primes, const int *odd_idx,
                size_t odd_len, size_t pos,
                int *idx_sel, size_t sel_len,
                seq_t cur, seq_t *best)
{
    if (pos == odd_len) {
        /* evaluate the constructed divisor */
        if (cur == (seq_t)1 || cur == (seq_t)2) {
            if (cur > *best) *best = cur;
            return;
        }
        if (!is_prime_small(cur)) {
            if (cur > *best) *best = cur;
        }
        return;
    }

    /* 1. skip this odd prime */
    dfs(odd_primes, odd_idx, odd_len, pos + 1,
        idx_sel, sel_len, cur, best);

    /* 2. include this odd prime once, if it keeps indices coprime */
    int idx = odd_idx[pos];
    bool ok = true;
    for (size_t k = 0; k < sel_len; ++k) {
        if (gcd_int_simple(idx, idx_sel[k]) > 1) {
            ok = false;
            break;
        }
    }
    if (ok) {
        idx_sel[sel_len] = idx;
        dfs(odd_primes, odd_idx, odd_len, pos + 1,
            idx_sel, sel_len + 1, cur * (seq_t)odd_primes[pos], best);
        /* no need to remove explicitly; sel_len will be restored by caller */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1 */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* separate exponent of 2, if present */
    uint64_t exp2 = 0;
    size_t odd_start = 0;
    if (pf_cnt > 0 && primes[0] == 2) {
        exp2 = exps[0];
        odd_start = 1;
    }

    /* precompute powers of 2 up to exp2 */
    seq_t pow2[65];
    pow2[0] = (seq_t)1;
    for (uint64_t e = 1; e <= exp2; ++e)
        pow2[e] = pow2[e - 1] * (seq_t)2;

    /* build list of primes up to n for binary search of indices */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)n, &prime_list, &prime_cnt);

    /* map odd primes to their indices */
    size_t odd_len = pf_cnt - odd_start;
    const uint64_t *odd_primes = &primes[odd_start];
    const uint64_t *odd_exps   = &exps[odd_start];
    int odd_indices[64];
    for (size_t i = 0; i < odd_len; ++i) {
        uint64_t p = odd_primes[i];
        size_t lo = 0, hi = prime_cnt;
        int idx = -1;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (prime_list[mid] == p) { idx = (int)(mid + 1); break; }
            if (prime_list[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        odd_indices[i] = idx; /* should always be found */
    }

    seq_t best = (seq_t)1;
    int idx_sel[64];
    size_t sel_len = 0;

    /* outer loop over all powers of 2 that divide n */
    for (uint64_t e = 0; e <= exp2; ++e) {
        seq_t base = pow2[e];
        dfs(odd_primes, odd_indices, odd_len,
            0, idx_sel, sel_len, base, &best);
    }

    free(prime_list);
    return best;
}
