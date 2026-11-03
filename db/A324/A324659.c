/*
 * A324659  a(n) = (1/2)*A318468(n), where A318468(n) is bitwise-AND of 2*n
 *          and sigma(n).
 * https://oeis.org/A324659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (2*n & sigma(n)) >> 1
 * Range:     u8 n <= 22, u16 n <= 2081, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9640 (2026-11-01)
 * Helpers:   factor_u64, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n) = product over prime factors p^e of (p^(e+1)-1)/(p-1) */
#include <stddef.h>
#include <stdint.h>
/* The following utility is available in the build environment: */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1; return 0 as safe fallback */

    /* Compute sigma(n) using prime factorization */
    seq_t sigma = 1;
    uint64_t primes[64];           /* Enough for n <= 10^4 */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned int e = (unsigned int)exps[i];
        seq_t num = ipow_i32(p, e + 1) - 1;   /* p^(e+1) - 1 */
        seq_t den = p - 1;                  /* p - 1 */
        seq_t term = num / den;             /* (p^(e+1)-1)/(p-1) */
        sigma *= term;
    }

    seq_t two_n = (seq_t)(2 * n);          /* 2*n as seq_t */
    seq_t result = (two_n & sigma) >> 1;   /* (2*n AND sigma(n)) / 2 */

    return result;
}
