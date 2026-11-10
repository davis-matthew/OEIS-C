/*
 * A354366  Denominators of Dirichlet inverse of primorial deflation fraction
 *          A319626(n) / A319627(n).
 * https://oeis.org/A354366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A064989(n) / gcd(A055615(n), A064989(n)); a(n) = 1 when n
 *            is not squarefree (mu(n)=0)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-10)
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

static idx_t A354366_prev_prime(idx_t p)
{
    idx_t q = p - 1;
    while (q > 2 && !is_prime(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > 1) return (seq_t)1;   /* mu(n)=0: gcd(0,d)=d, so a(n)=d/d=1 */

    seq_t d = 1;          /* A064989(n) for squarefree n */
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        d *= (seq_t)A354366_prev_prime((idx_t)pr[i]);
    }

    /* squarefree: gcd(|n*mu(n)|, d) = gcd(n, d) */
    seq_t g = gcd((seq_t)n, d);
    return d / g;
}
