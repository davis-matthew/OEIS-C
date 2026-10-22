/*
 * A347386  Number of iterations of A347385 (Dedekind psi function applied to
 *          the odd part of n) needed to reach a power of 2.
 * https://oeis.org/A347386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=0 if odd_part(n)=1 else 1 + a(psi(odd_part(n)))
 * Range:     u8 n <= 1137, u16 n <= 8987, u32 n <= 8987, u64 n <= 8987,
 *            u128 n <= 8987, big any n
 * Helpers:   factor_u64, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/odd_part.h"
#include "../utils/factor_u64.h"

/* Compute Dedekind ψ for an odd 64‑bit integer. */
static seq_t psi_of_odd_uint64(uint64_t m)
{
    seq_t res = (seq_t)m;
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        res = res / (seq_t)p * (seq_t)(p + 1);
    }
    return res;
}

/* Sequence A347386: number of iterations of the Dedekind ψ
   applied to the odd part of n needed to reach a power of 2. */
seq_t a(idx_t n)
{
    seq_t odd = odd_part((seq_t)n);
    if (odd == 1)
        return 0;

    /* odd is <= n <= UINT64_MAX, safe to cast */
    uint64_t odd_val = (uint64_t)odd;
    seq_t next = psi_of_odd_uint64(odd_val);
    return (seq_t)1 + a((idx_t)next);
}
