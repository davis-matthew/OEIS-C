/*
 * A327572  Partial sums of an infinitary analog of Euler's phi function: a(n)
 *          = Sum_{k=1..n} iphi(k), where iphi is A091732.
 * https://oeis.org/A327572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} iphi(k), where iphi(k) is A091732: iphi(k) =
 *            ∏_{p^e‖k} ∏_{j: 2^j‖e} (p^{2^j} – 1)
 * Range:     u8 n <= 27, u16 n <= 445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute the infinitary φ function for a single number. */
static seq_t iphi(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                   /* iphi(1) = 1 */
    uint64_t primes[16], exps[16];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);    /* factor n */

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part = (seq_t)1;
        for (int j = 0; e >> j; ++j) {
            if ((e >> j) & 1ULL) {          /* bit j of e is set */
                seq_t pw = (seq_t)p;
                for (int l = 0; l < j; ++l)   /* pw = p^(2^j) */
                    pw *= pw;
                seq_t term = pw - (seq_t)1;
                part *= term;
            }
        }
        result *= part;
    }
    return result;
}

/* Partial sums of the infinitary φ function.  Values are memoised. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    if (n == 0)
        return (seq_t)0;

    if (n > max_computed) {                 /* expand cache if necessary */
        size_t new_cap = (size_t)n + 1;
        if (cap < new_cap) {
            seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
            if (!new_cache)
                abort();                    /* allocation failure */
            for (size_t i = cap; i < new_cap; ++i)
                new_cache[i] = (seq_t)0;
            cache = new_cache;
            cap = new_cap;
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + iphi(i);
        }
        max_computed = n;
    }
    return cache[n];
}
