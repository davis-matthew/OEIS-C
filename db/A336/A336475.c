/*
 * A336475  Multiplicative with a(2^e) = 1, and for odd primes p, a(p^e) =
 *          (e+1)*p^e.
 * https://oeis.org/A336475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p|n, p odd} (e_p + 1) * p^e_p (for 2^e, a=1)
 * Range:     u8 n <= 44, u16 n <= 3464, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses factorization and pow_uint from utils */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Return 1 for n = 1 or any power of 2 */
    if (n == 0) return 0; /* Undefined for 0 but safe guard */

    /* Temporary storage for prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 2) continue;          /* multiplicative factor is 1 for powers of 2 */
        seq_t val = pow_uint(p, (idx_t)e);          /* p^e   */
        val = val * (seq_t)(e + 1);                  /* (e+1)*p^e */
        result = result * val;                      /* multiply into result */
    }
    return result;
}
