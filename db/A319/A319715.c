/*
 * A319715  Sum of A276150(d) over divisors d of n, where A276150 gives the
 *          sum of digits in primorial base.
 * https://oeis.org/A319715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} A276150(d) where A276150(d) is the sum of the
 *            digits of d expressed in primorial base
 * Range:     u8 n <= 5039, u16 n <= 8804, u32 n <= 8804, u64 n <= 8804,
 *            u128 n <= 8804, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

/* Return the next prime greater than start (small primes only). */
static uint64_t next_prime_uint64(uint64_t start)
{
    uint64_t cand = start + 1;
    while (!is_prime_small(cand))
        ++cand;
    return cand;
}

/* Sum of the digits of n in primorial base. */
static seq_t primorial_digit_sum(uint64_t n)
{
    seq_t sum = 0;
    uint64_t P = 1;          /* product of primes processed so far */
    uint64_t prime = 2;      /* current prime (radix) */

    while (1) {
        uint64_t digit = (n / P) % prime;
        sum += (seq_t)digit;

        /* If P > n after this step, all further digits will be zero. */
        if (P > n)
            break;

        /* Advance to next radix. */
        if (P > UINT64_MAX / prime)   /* overflow guard, not needed for small n */
            break;
        P *= prime;
        prime = next_prime_uint64(prime);
    }
    return sum;
}

/* Recursive generation of all divisors of n and accumulation of A276150(d). */
static void gen_divisors(size_t idx, uint64_t cur,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        *sum += primorial_digit_sum(cur);
        return;
    }
    uint64_t val = cur;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, primes, exps, cnt, sum);
        if (e < exps[idx]) {
            if (val > UINT64_MAX / primes[idx])
                break;   /* safety, not expected for n up to 10⁴ */
            val *= primes[idx];
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Factorize n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t total = 0;
    gen_divisors(0, (uint64_t)1, primes, exps, cnt, &total);
    return total;
}
