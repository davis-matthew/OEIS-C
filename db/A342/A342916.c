/*
 * A342916  a(n) = (1+n) / gcd(1+n, A001615(n)), where A001615 is Dedekind
 *          psi, n * Product_{p|n, p prime} (1 + 1/p).
 * https://oeis.org/A342916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) / gcd(n+1, ψ(n)) where ψ(n) = n * Π_{p|n} (1 + 1/p)
 *            = n * Π_{p|n} (p+1)/p (Dedekind psi function)
 * Range:     u8 n <= 254, u16 n <= 9707, u32 n <= 9707, u64 n <= 9707,
 *            u128 n <= 9707, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1; for n==0 the behaviour is unspecified. */

    /* Factor n.  We only need the distinct primes, but factor_u64
       will provide them in pr[]; ex[] contains exponents but is
       ignored.  The number of distinct primes is small for n <= 64-bit. */
    static uint64_t pr[20], ex[20];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* Compute divisor = Π p_i and numerator = Π (p_i + 1). */
    uint64_t divisor = 1;
    seq_t   numerator = 1;

    for (size_t i = 0; i < cnt; ++i) {
        divisor *= pr[i];
        numerator *= (seq_t)(pr[i] + 1);
    }

    /* ψ(n) = (n / divisor) * numerator  (divisor divides n). */
    seq_t part = (seq_t)(n / divisor);
    seq_t psi  = part * numerator;

    /* a(n) = (n+1) / gcd(n+1, ψ(n)) */
    seq_t n1 = (seq_t)(n + 1);
    seq_t g  = gcd(n1, psi);
    return n1 / g;
}
