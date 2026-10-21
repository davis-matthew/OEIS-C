/*
 * A293902  If n = p_1^e_1 * ... * p_k^e_k, p_1, ..., p_k primes, then a(n) =
 *          Product c! where c ranges over products of all combinations of
 *          exponents e_1, ..., e_k as {e_1, e_1*e_2, e_1*e_3, e_2*e_3,
 *          e_1*e_2*e_3, ..., e_1*e_2*...*e_k}.
 * https://oeis.org/A293902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ factorial(∏ e_i for every non‑empty subset of the
 *            exponents of n), with an additional factor factorial(1) for the
 *            empty subset
 * Range:     u8 n <= 47, u16 n <= 143, u32 n <= 431, u64 n <= 1727,
 *            u128 n <= 8639, big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 1)                /* a(1) = 1 by definition */
        return (seq_t)1;

    /* The number of distinct prime factors of n ≤ 60 for 64‑bit n,
       so static arrays of size 64 are safe. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 pre-fills primes[] and exps[] with the factorisation
       of n and stores the number of prime factors in cnt. */
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    /* Include the empty subset (1)! only for completeness; it is 1
       so it does not affect the product, but we keep it for clarity. */
    result *= factorial(1);

    /* Iterate over all non‑empty subsets of the exponent list.  */
    if (cnt > 0) {
        /* 1ULL << cnt is safe because cnt ≤ 60 for 64‑bit n. */
        size_t total = (size_t)1 << cnt;
        for (size_t mask = 1; mask < total; ++mask) {
            uint64_t prod = 1;
            for (size_t i = 0; i < cnt; ++i)
                if (mask & ((size_t)1 << i))
                    prod *= exps[i];
            result *= factorial(prod);
        }
    }

    return result;
}
