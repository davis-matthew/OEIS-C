/*
 * A241919  If n is a prime power, p_i^e, a(n) = i, (with a(1)=0), otherwise
 *          difference (i-j) of the indices of the two largest distinct primes
 *          p_i, p_j, i > j in the prime factorization of n: a(n) = A061395(n)
 *          - A061395(A051119(n)).
 * https://oeis.org/A241919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061395(n) - A061395(A051119(n))
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;               /* a(1) = 0 */

    /* factor the integer (uint64_t) */
    uint64_t x = (uint64_t)n;
    uint64_t primes[16];        /* enough for any 64‑bit number */
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    if (cnt == 1)
        /* n is a prime power: return index of its prime factor */
        return compute_pi(primes[0]);

    /* primes are returned in ascending order.
       The two largest distinct primes are primes[cnt-1] and primes[cnt-2] */
    seq_t i = compute_pi(primes[cnt - 1]); /* index of largest prime */
    seq_t j = compute_pi(primes[cnt - 2]); /* index of second largest prime */

    return i - j;                       /* difference of indices */
}
