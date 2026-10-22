/*
 * A308053  Coreful abundant numbers: numbers k such that csigma(k) > 2*k,
 *          where csigma(k) is the sum of the coreful divisors of k (A057723).
 * https://oeis.org/A308053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coreful abundant numbers:
   a(n) = the nth integer k such that the sum of its coreful divisors csigma(k)
   satisfies csigma(k) > 2*k.
   The coreful divisor sum is
       csigma(k) = ∏_{p^e‖k} (p + p² + … + p^e),
   where the product runs over the prime powers in the factorisation of k.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 for safety */

    idx_t count = 0;
    for (uint64_t k = 1; ; ++k) {
        /* Factorise k.  Arrays are large enough for all prime factors of a 64‑bit integer. */
        uint64_t pr[64], ex[64];
        size_t cnt = 0;
        factor_u64(k, pr, ex, &cnt);

        /* Compute csigma(k) = ∏ (p + p² + … + p^e) */
        seq_t csig = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p = (seq_t)pr[i];
            seq_t power = p;
            seq_t sum   = power;      /* p¹ */

            for (uint64_t e = 2; e <= ex[i]; ++e) {
                power *= p;
                sum += power;
            }
            csig *= sum;
        }

        /* Test coreful abundant: csig > 2 * k */
        if (csig > (seq_t)2 * (seq_t)k) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
