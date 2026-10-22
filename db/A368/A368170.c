/*
 * A368170  The largest cubefull exponentially odd divisor of n.
 * https://oeis.org/A368170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is the largest cubefull exponentially odd divisor of n.
 *            Multiplicative definition: a(p^e) = 1 if e ≤ 2, p^e if e > 2 and
 *            e odd, p^(e‑1) if e > 2 and e even
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/pow_seq.h"     /* pow_seq(seq_t, int) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;               /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;
    /* maximum number of distinct prime factors for a 64‑bit integer is < 64 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        if (e <= 2)
            continue;   /* contributes 1 */

        seq_t term;
        if (e & 1ULL)          /* odd exponent (>1) -> use full power */
            term = pow_seq((seq_t)p, (int)e);
        else                   /* even exponent (>2) -> use one less power */
            term = pow_seq((seq_t)p, (int)(e - 1));

        result *= term;
    }

    return result;
}
