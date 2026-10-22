/*
 * A329617  Product of distinct exponents of prime factors of A108951(n),
 *          where A108951 is fully multiplicative with a(prime(i)) = prime(i)#
 *          = Product_{i=1..i} A000040(i).
 * https://oeis.org/A329617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct exponents of prime factors of
 *            A108951(n), where A108951 is fully multiplicative with
 *            A108951(p_k) = primorial(p_k) = ∏_{i=1..k} p_i
 * Range:     u8 n <= 9093, u16 n <= 9093, u32 n <= 9093, u64 n <= 9093,
 *            u128 n <= 9093, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product of distinct suffix sums of exponents of n. */
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety guard, offset starts at 1 */

    /* Factor n: primes[] contains the primes in ascending order,
       exps[] the corresponding exponents. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute distinct exponents which are the suffix sums
       of the exponents array. */
    seq_t product = 1;
    seq_t seen[20];
    size_t seen_cnt = 0;

    seq_t tail = 0;
    for (size_t i = cnt; i > 0; --i) {
        tail += (seq_t)exps[i - 1];
        bool already = false;
        for (size_t j = 0; j < seen_cnt; ++j) {
            if (seen[j] == tail) {
                already = true;
                break;
            }
        }
        if (!already) {
            seen[seen_cnt++] = tail;
            product *= tail;
        }
    }

    /* For n = 1 (cnt = 0), product remains 1, which is the correct
       value a(1) = 1. */
    return product;
}
