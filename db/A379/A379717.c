/*
 * A379717  The second Jordan totient function applied to the cubefree
 *          numbers.
 * https://oeis.org/A379717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007434(A004709(n))
 * Range:     u8 n <= 14, u16 n <= 214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static idx_t last_candidate   = 1;   /* next integer to test for cubefreeness   */
static idx_t computed_terms   = 0;   /* number of terms already computed        */
static seq_t *values          = NULL;/* cached sequence values                   */
static size_t values_cap      = 0;   /* capacity of 'values' array               */

/* Return true iff 'num' is cubefree (no prime exponent >= 3). */
static inline bool
is_cubefree(idx_t num)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)num, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 3)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, guard for completeness */

    /* Ensure storage capacity for 'n' terms */
    if (values_cap < (size_t)n) {
        size_t new_cap = values_cap ? values_cap * 2 : 4;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* allocation failure */
        values = tmp;
        values_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= computed_terms)
        return values[n - 1];

    /* Generate terms until we reach the requested index */
    while (computed_terms < n) {
        if (is_cubefree(last_candidate)) {
            /* Compute J_2(last_candidate) */
            seq_t m_sq = (seq_t)last_candidate * (seq_t)last_candidate;
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64((uint64_t)last_candidate, primes, exps, &cnt);
            for (size_t i = 0; i < cnt; ++i) {
                seq_t p_sq = (seq_t)primes[i] * (seq_t)primes[i];
                m_sq = (m_sq / p_sq) * (p_sq - 1);
            }
            values[computed_terms] = m_sq;
            ++computed_terms;
        }
        ++last_candidate;
    }

    return values[n - 1];
}
