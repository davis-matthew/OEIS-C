/*
 * A318465  The number of Zeckendorf-infinitary divisors of n = Product_{i}
 *          p(i)^r(i): divisors d = Product_{i} p(i)^s(i), such that the
 *          Zeckendorf expansion (A014417) of each s(i) contains only terms
 *          that are in the Zeckendorf expansion of r(i).
 * https://oeis.org/A318465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^{sum over prime–power exponents e of the number of
 *            terms in the Zeckendorf representation of e}. See OEIS A318465
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9899 (2026-11-01)
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

static int zeckendorf_term_count(uint64_t e)
{
    /* Pre‑computed Fibonacci numbers F1=1, F2=2, F3=3, ...  (Zeckendorf base) */
    static uint64_t fibs[93];
    static int fibcnt = 0;
    if (fibcnt == 0) {
        fibs[0] = 1;
        fibs[1] = 2;
        fibcnt   = 2;
        while (1) {
            uint64_t next = fibs[fibcnt-1] + fibs[fibcnt-2];
            if (next < fibs[fibcnt-1]) break;      /* overflow protection */
            fibs[fibcnt++] = next;
            if (next > ((uint64_t)0xFFFFFFFFFFFFFFFFULL)) break;
            if (fibcnt >= 93) break;
        }
    }
    if (e == 0) return 0;
    int count = 0;
    for (int i = fibcnt - 1; i >= 0; --i) {
        if (e >= fibs[i]) {
            e -= fibs[i];
            ++count;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for offsets <1, return 0 for safety */

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute exponent sum of 2 using Zeckendorf term counts of each exponent */
    int total_terms = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total_terms += zeckendorf_term_count(exps[i]);
    }

    /* a(n) = 2^{total_terms} */
    return pow2((idx_t)total_terms);
}
