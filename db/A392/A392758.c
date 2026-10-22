/*
 * A392758  Number of divisors of n which are not primorials.
 * https://oeis.org/A392758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n minus the number of primorials
 *            dividing n. The number of primorials dividing n equals 1 (for 1)
 *            plus the number of consecutive primes starting from 2 that all
 *            divide n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset 1, but harmless for n==0 */

    uint64_t N = (uint64_t)n;           /* idx_t is uint64_t */

    /* divisor count of n */
    seq_t divcnt = (seq_t)divisor_count_u64(N);

    /* factor n; store prime exponents (existing but not needed here). */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(N, pr, ex, &cnt);

    /* count primorials dividing n: 1 for 1, plus consecutive primes starting from 2 */
    seq_t prim_count = 1;                /* 1 is always a divisor */
    size_t idx = 0;
    uint64_t next_prime = 2;

    while (1) {
        /* advance index to the first factor prime >= next_prime */
        while (idx < cnt && pr[idx] < next_prime)
            ++idx;

        if (idx < cnt && pr[idx] == next_prime) {
            /* this prime is present */
            ++prim_count;

            /* find next prime after current */
            ++next_prime;
            while (!is_prime((idx_t)next_prime))
                ++next_prime;
        } else {
            /* missing prime; stop counting consecutive primes */
            break;
        }
    }

    return divcnt - prim_count;
}
