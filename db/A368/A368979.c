/*
 * A368979  The number of exponential divisors of n that are exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A368979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368979: a(n) = ∏_{p^e‖n} A001227(e)
   where A001227(k) = number of odd divisors of k.
   The sequence is multiplicative and for each prime power
   the factor is the count of odd divisors of its exponent. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Helper: compute number of odd divisors of an exponent e. */
static seq_t odd_divisor_count(seq_t e)
{
    if (e == 1)
        return 1;

    /* Remove all factors of 2 from e, since they do not contribute
       odd divisors. */
    idx_t temp = (idx_t)e;
    while ((temp & 1) == 0)
        temp >>= 1;

    if (temp == 1)
        return 1;

    /* Factor the odd part of e.  Since e ≤ log₂(n), it is tiny
       and we can use a small static buffer. */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt = 0;
    factor_u64((uint64_t)temp, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= (seq_t)(ex[i] + 1);  /* (exponent + 1) from divisor count */

    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;  /* a(1) = 1 */

    /* Factor n into prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t oddcnt = odd_divisor_count((seq_t)exps[i]);
        res *= oddcnt;
    }
    return res;
}
