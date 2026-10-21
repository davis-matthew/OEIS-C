/*
 * A249140  To obtain a(n), write the n-th composite number as a product of
 *          primes, subtract 1 from each prime and multiply.
 * https://oeis.org/A249140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of (p-1) for each prime factor p of the n-th
 *            composite number
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    a(n) = ∏_{p|N(n)} (p−1)   where N(n) is the n-th composite number, counting from 4. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* Find the n-th composite number */
    idx_t  count = 0;
    uint64_t num = 4;               /* first composite number */
    for (;; ++num) {
        if (!is_prime(num)) {
            ++count;
            if (count == n) break;
        }
    }

    /* Factor the composite number */
    uint64_t primes[64] = {0};
    uint64_t exps [64] = {0};
    size_t   cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    /* Compute the product of (prime-1) for each occurrence */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(primes[i] - 1);
        for (uint64_t k = 0; k < exps[i]; ++k)
            result *= term;
    }

    return result;
}
