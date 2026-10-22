/*
 * A344695  a(n) = gcd(sigma(n), psi(n)), where sigma is the sum of divisors
 *          function, A000203, and psi is the Dedekind psi function, A001615.
 * https://oeis.org/A344695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), psi(n))
 * Range:     u8 n <= 137, u16 n <= 8832, u32 n <= 8832, u64 n <= 8832,
 *            u128 n <= 8832, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */

/* Dedekind ψ function: ψ(n) = ∏_{p^e‖n} p^{e-1}·(p+1) */
static seq_t dedekind_psi(uint64_t n)
{
    size_t max_facs = 64;          /* enough for any 64‑bit integer */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    seq_t psi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        size_t e = ex[i];

        /* multiply by p^(e-1) if e>1 */
        if (e > 1) {
            seq_t powp = 1;
            for (size_t k = 1; k < e; ++k)
                powp *= p;
            psi *= powp;
        }
        /* multiply by (p+1) */
        psi *= (p + (seq_t)1);
    }
    return psi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for 0, defensive */

    uint64_t n64 = (uint64_t)n;
    seq_t sigma = sigma_of(n);
    seq_t psi   = dedekind_psi(n64);
    return gcd(sigma, psi);
}
