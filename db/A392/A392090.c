/*
 * A392090  The unitary totient of the smallest cubefull number that is
 *          divisible by n.
 * https://oeis.org/A392090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p^max(e,3) – 1) (unitary totient of the
 *            smallest cubefull number divisible by n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used, but safe */

    /* arrays for prime factors; uint64_t can hold all primes of a 64‑bit integer */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    if (n > 1)
        factor_u64((uint64_t)n, primes, exps, &cnt);   /* factor n */

    seq_t result = (seq_t)1;  /* empty product = 1 (covers n = 1) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t k = (e < 3 ? 3ULL : e);   /* max(e,3) */

        /* Compute p^k in seq_t without overflow for small k (k ≤ 63 for uint64_t n) */
        seq_t power = (seq_t)1;
        seq_t base  = (seq_t)primes[i];
        for (uint64_t j = 0; j < k; ++j)
            power *= base;

        seq_t term = power - (seq_t)1;   /* p^k – 1 */
        result *= term;
    }

    return result;
}
