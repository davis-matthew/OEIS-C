/*
 * A348949  a(n) = A003959(A276086(n)), where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e, and A276086 gives the prime product form of
 *          primorial base expansion of n.
 * https://oeis.org/A348949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A003959(A276086(n))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 207, u64 n <= 2096,
 *            u128 n <= 8494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Small helper: primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2U) return false;
    if (n % 2U == 0U) return n == 2U;
    for (uint64_t i = 3U; i * i <= n; i += 2U)
        if (n % i == 0U) return false;
    return true;
}

/* Return the next prime greater than `n`. */
static uint64_t next_prime_u64(uint64_t n)
{
    uint64_t cand = n + 1U;
    while (!is_prime_u64(cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1U;           /* a(0) = 1 */

    seq_t result = 1U;
    idx_t weight = 1U;                /* w0 = 1 */
    uint64_t prime = 2U;              /* p0 = 2 */

    while (weight <= n)
    {
        uint64_t digit = (n / weight) % prime;

        if (digit != 0U)
        {
            /* multiply by (prime+1)^digit */
            for (uint64_t i = 0U; i < digit; ++i)
                result *= (seq_t)(prime + 1U);
        }

        /* update the weight for the next position
           stop if multiplication would overflow idx_t or exceed n   */
        if (weight > n / prime)
            break;
        weight = weight * prime;

        /* move to the next prime in the primorial base */
        prime = next_prime_u64(prime);
    }

    return result;
}
