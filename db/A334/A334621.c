/*
 * A334621  Number of unitary prime divisors, p, of n such that n-p is
 *          squarefree.
 * https://oeis.org/A334621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p|n, p prime, gcd(p,n/p)=1} μ(n-p)^2
 * Range:     u8 n <= 7237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"

/* Count of unitary prime divisors of n such that n-p is squarefree. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* no prime divisors for n < 2 */

    /* factor n: we reuse static buffers to avoid repeated allocation */
    static uint64_t primes[64];
    static uint64_t exps[64];
    static size_t cnt = 0;

    /* factor_u64 expects uint64_t; cast n accordingly */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 1) {                    /* unitary prime divisor */
            uint64_t p = primes[i];
            seq_t m = (seq_t)n - (seq_t)p;     /* n - p */
            if (m > (seq_t)0 && is_squarefree(m)) {
                result += (seq_t)1;
            }
        }
    }
    return result;
}
