/*
 * A253628  a(n) = psi(n) mod n, where psi is the Dedekind psi function
 *          (A001615).
 * https://oeis.org/A253628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ψ(n) mod n where ψ(n) = n * Π_{p|n} (1 + 1/p)
 * Range:     u8 n <= 307, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle offset: a(1) corresponds to n = 1.  n = 0 is undefined. */
    if (n == 0) return 0;

    /* ψ(n) starts as n.  Subsequent factors (p+1)/p are applied for
       every distinct prime divisor p of n. */
    seq_t psi = (seq_t)n;

    /* For n ≤ 2^64 a small array suffices for the prime factor list. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        psi = (psi / p) * (p + 1);
    }

    return psi % (seq_t)n;
}
