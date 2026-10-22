/*
 * A323410  Unitary analog of cototient function A051953: a(n) = n -
 *          A047994(n).
 * https://oeis.org/A323410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - ∏_{p^e∥n} (p^e - 1)
 * Range:     u8 n <= 389, u16 n <= 8254, u32 n <= 8254, u64 n <= 8254,
 *            u128 n <= 8254, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* a(0) is unused, but return 0 safely */
    if (n == 0) return 0;

    /* φ*(1) = 1 */
    seq_t phi = 1;

    if (n > 1) {
        /* factor_u64 will fill primes[] and exps[] with the prime base
           and exponent of each distinct prime power p^e dividing n.     */
        uint64_t primes[64];
        uint64_t exps  [64];
        size_t   cnt   = 0;

        factor_u64(n, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            /* compute p^e - 1 and multiply into φ*(n) */
            seq_t term = pow_uint(primes[i], exps[i]);  /* p^e   */
            term -= (seq_t)1;                          /* p^e-1 */
            phi *= term;
        }
    }

    return (seq_t)n - phi;
}
