/*
 * A383575  Characteristic function of numbers of the form k = m^e, where m is
 *          squarefree and (-1)^omega(k) = mu(e).
 * https://oeis.org/A383575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = m^e with m squarefree, e squarefree, and
 *            parity(ω(m)) = parity(ω(e)); otherwise 0. Otherwise a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/mu_of.h"          /* int mu_of(idx_t d) */
#include "../utils/count_distinct_primes.h" /* int count_distinct_primes(uint64_t n) */

seq_t a(idx_t n) {
    /* 1 is defined to be part of the sequence. */
    if (n == 1) return (seq_t)1;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* if n had no prime factors (i.e. n==1), already handled */
    if (cnt == 0) return (seq_t)1;

    /* all exponents must be equal */
    uint64_t e = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] != e) return (seq_t)0;
    }

    /* e must be squarefree: μ(e) != 0 */
    if (mu_of(e) == 0) return (seq_t)0;

    /* parity of ω(m) equals parity of ω(e) */
    unsigned int parity_m = (unsigned int)(cnt & 1);                 /* ω(m) = cnt */
    unsigned int parity_e = (unsigned int)(count_distinct_primes(e) & 1);

    return (seq_t)((parity_m == parity_e) ? 1 : 0);
}
