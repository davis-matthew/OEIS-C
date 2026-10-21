/*
 * A247572  Base-11 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247572
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * ord_{n}(11) for n % 11 ≠ 0, otherwise a(n) = n + 1.
 *            Here ord_{n}(11) is the smallest positive k such that 11^k ≡ 1
 *            (mod n). This implements the base‑11 state complexity for the
 *            periodic sequence (123…n)*
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* the function is only defined for n ≥ 2 – behaviour for smaller n is irrelevant */
    if (n % 11 == 0) {
        return (seq_t)n + 1;
    }

    seq_t order = 1;
    seq_t power = 11 % (seq_t)n;   /* 11 mod n, where gcd(11,n)=1 here */

    while (power != 1) {
        /* power and 11 are < n ≤ 2^64-1, so we can safely cast to uint64_t
           and use the provided mod_mul routine to avoid overflow.          */
        uint64_t p   = (uint64_t)power;
        uint64_t nxt = mod_mul(p, 11, (uint64_t)n);
        power = (seq_t)nxt;
        order++;
    }

    return (seq_t)n * order;
}
