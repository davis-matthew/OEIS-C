/*
 * A351301  a(n) = n^6 * Product_{p|n, p prime} (1 + 1/p^6).
 * https://oeis.org/A351301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^6 * Product_{p|n, p prime} (1 + 1/p^6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1621,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Simple exponentiation for seq_t: base^exp */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1 */

    /* Factor n into primes and exponents */
    const size_t MAX_FACTS = 64;          /* plenty for 64‑bit n */
    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        idx_t e = (idx_t)exps[i];
        seq_t p = (seq_t)primes[i];

        seq_t pow6e         = pow_seq(p, 6 * e);          /* p^(6e)   */
        seq_t pow6e_minus6  = pow_seq(p, 6 * e - 6);      /* p^(6e-6) */

        result *= (pow6e + pow6e_minus6);
    }

    return result;
}
