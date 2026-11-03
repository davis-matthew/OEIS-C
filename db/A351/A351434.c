/*
 * A351434  If n = Product (p_j^k_j) then a(n) = Product ((p_j - 1)^(k_j +
 *          1)).
 * https://oeis.org/A351434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^k||n} (p-1)^(k+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9686 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for 0, but safe */
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    uint64_t N = (uint64_t)n;

    /* arrays large enough for prime factorizations of 64‑bit numbers */
    uint64_t prime[64];
    uint64_t exp[64];
    size_t cnt;

    factor_u64(N, prime, exp, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)(prime[i] - 1);
        seq_t power = ipow(base, (seq_t)(exp[i] + 1));
        result = result * power;
    }

    return result;
}
