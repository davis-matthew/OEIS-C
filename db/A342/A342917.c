/*
 * A342917  a(n) = A001615(n) / gcd(1+n, A001615(n)), where A001615 is
 *          Dedekind psi, n * Product_{p|n, p prime} (1 + 1/p).
 * https://oeis.org/A342917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Dedekind ψ(n) / gcd(1+n, Dedekind ψ(n))
 * Range:     u8 n <= 119, u16 n <= 8463, u32 n <= 8463, u64 n <= 8463,
 *            u128 n <= 8463, big any n
 * Helpers:   factor_u64, seq_gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute Dedekind ψ(n) = ∏_{p^e‖n} p^{e-1} · (p+1) */
    seq_t psi = 1;
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exponents, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = seq_pow((seq_t)primes[i], (idx_t)(exponents[i] - 1));
        term = seq_mul(term, (seq_t)(primes[i] + 1));
        psi   = seq_mul(psi, term);
    }

    seq_t g = seq_gcd(psi, (seq_t)n + 1);
    return psi / g;
}
