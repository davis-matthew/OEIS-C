/*
 * A307380  Number of quintic primitive Dirichlet characters modulo n.
 * https://oeis.org/A307380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative function; a(p^e) = 4 if p^e = 25 or (p ≡ 1 mod 5
 *            and e = 1), otherwise 0
 * Range:     u8 n <= 9782, u16 n <= 9782, u32 n <= 9782, u64 n <= 9782,
 *            u128 n <= 9782, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factor_u64 writes into these arrays; 64 is safe for 64‑bit n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        if (p == 5) {                 /* 5^e case */
            if (e != 2)
                return (seq_t)0;
        } else {                       /* other primes */
            if (e != 1 || (p % 5) != 1)
                return (seq_t)0;
        }

        res *= (seq_t)4;
    }

    return res;
}
