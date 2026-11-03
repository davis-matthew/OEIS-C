/*
 * A292786  a(n) = psi(n) - phi(n).
 * https://oeis.org/A292786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = ψ(n) – φ(n) (Dedekind ψ of n minus Euler's φ)
 * Range:     u8 n <= 119, u16 n <= 24569, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 94 terms through n = 8506 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Factor n into primes p^e */
    uint64_t u = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(u, pr, ex, &cnt);

    /* Compute ψ(n) = ∏ (p^e + p^(e‑1)) */
    seq_t psi = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pe   = ipow_u64_idx(pr[i], (idx_t)ex[i]);           /* p^e          */
        seq_t term = pe + (pe / (seq_t)pr[i]);               /* p^e + p^(e-1)*/
        psi *= term;
    }

    /* Compute φ(n) */
    seq_t phi_val = totient_seq((seq_t)n);

    /* Return the difference ψ(n) – φ(n) */
    return psi - phi_val;
}
