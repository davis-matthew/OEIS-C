/*
 * A306328  If n = Product (p_j^k_j) then a(n) = Sum (p_j)^Product (k_j).
 * https://oeis.org/A306328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of n raised to the *
 *            product of their exponents. Equivalently, a(n) =
 *            sopf(n)^tau(n/rad(n))
 * Range:     u8 n <= 35, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;  // a(1) = 0

    uint64_t nn = (uint64_t)n;

    /* arrays large enough for the number of distinct primes of a 64‑bit
       integer; 64 is more than sufficient (the maximum possible is 16
       for 64‑bit numbers). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorize nn into primes[] and exps[].  cnt points to the number
       of distinct prime factors found. */
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        sum += (seq_t)primes[i];          /* sum of distinct primes   */
        prod *= (seq_t)exps[i];           /* product of exponents     */
    }

    /* compute (sum)^(prod).  big_pow can handle arbitrary precision
       when seq_t is an arbitrary‑precision type. */
    return ipow(sum, prod);
}
