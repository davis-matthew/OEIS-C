/*
 * A349172  a(n) = Sum_{d|n} psi(d) * A003959(n/d), where A003959 is fully
 *          multiplicative with a(p) = (p+1), and psi is Dedekind psi
 *          function, A001615.
 * https://oeis.org/A349172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} psi(d) * A003959(n/d) a is multiplicative with
 *            a(p^e) = (p+2)*(p+1)^e - (p+1)*p^e
 * Range:     u8 n <= 15, u16 n <= 383, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 9757 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 1)           /* undefined for n = 0, return 0 as a safeguard */
        return (seq_t)0;
    if (n == 1)          /* a(1) = 1 */
        return (seq_t)1;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    /* For each prime power multiply its contribution */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e = (idx_t)exps[i];

        seq_t pow_p1 = ipow_idx((seq_t)(p + 1), e);   /* (p+1)^e */
        seq_t pow_p  = ipow_idx((seq_t)p, e);         /* p^e   */

        seq_t term1 = ((seq_t)(p + 2)) * pow_p1;
        seq_t term2 = ((seq_t)(p + 1)) * pow_p;

        seq_t prime_term = term1 - term2;

        result *= prime_term;
    }

    return result;
}
