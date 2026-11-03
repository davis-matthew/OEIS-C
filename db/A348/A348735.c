/*
 * A348735  Denominator of Product((p+1)^e / ((p^e)+1)), when n =
 *          Product(p^e), with p primes, and e their exponents.
 * https://oeis.org/A348735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ∏ᵖᵉ (p+1)⁽ᵉ⁾ / (pᵉ+1) , i.e. a(n) =
 *            A003959(n) / gcd(A003959(n), A034448(n)) (n ≥ 1)
 * Range:     u8 n <= 47, u16 n <= 21125, u32 n <= 21125, u64 n <= 21125,
 *            u128 n <= 21125, big any n
 * Validated: standard, pass, 116 terms through n = 9807 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* value for out‑of‑offset indices */

    /* factor n: primes[] and exps[] store prime factors and exponents */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n, primes, exps, &cnt);

    seq_t num = 1;   /* A034448(n) = ∏ (p+1)^e  */
    seq_t den = 1;   /* A003959(n)  = ∏ (p^e + 1) */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t  e = (idx_t)exps[i];

        /* numerator part: (p+1)^e */
        num *= ipow_idx(p + 1, e);

        /* denominator part: p^e + 1 */
        seq_t pe = ipow_idx(p, e);
        den *= (pe + 1);
    }

    seq_t g = gcd(num, den);
    return den / g;  /* the reduced denominator */
}
