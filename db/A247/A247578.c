/*
 * A247578  Base-17 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247578
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   - If 17 | n then a(n) = n + n/17 . - Otherwise let n = 2^e * m
 *            with m odd. Let m = ∏ p_i^α_i, where the p_i are distinct odd
 *            primes. Let ω(m) be the number of distinct odd primes. For each
 *            odd prime p_i ≠ 17 define t(p_i) = (p_i-1)/2 if 17 is a
 *            quadratic residue mod p_i (i.e. (17|p_i) = 1), other...
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factor_u64, is_quadratic_residue_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_quadratic_residue_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n<2 */

    /* Special treatment for multiples of 17 */
    if (n % 17 == 0)
        return (seq_t)n + (seq_t)(n / 17);

    /* Isolate the odd part (no factor 2) */
    idx_t m = n;
    while ((m & 1) == 0)                 /* divide by 2^e */
        m >>= 1;

    /* Factor the odd part */
    /* 64 is ample for n <= 10000, all prime factors fit */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Product over distinct odd primes (p ≠ 17) */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 17) continue;          /* p≠17 by the preceding test */
        seq_t term = (seq_t)(p - 1);      /* p-1  */
        if (is_quadratic_residue_prime(p))
            term >>= 1;                  /* divide by 2 when 17 is a residue */
        prod *= term;
    }

    /* Divide by 2^(ω(m)-1)  – ω(m)=cnt  */
    if (cnt > 1)
        prod >>= (cnt - 1);              /* exact division by a power of 2 */

    /* Final result: a(n) = n * prod */
    return (seq_t)n * prod;
}
