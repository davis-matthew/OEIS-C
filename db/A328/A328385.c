/*
 * A328385  If n is of the form p^p, a(n) = n, otherwise a(n) is the first
 *          number found by iterating the map x -> A003415(x) that is
 *          different from n and either a prime, or whose degree (A051903)
 *          differs from the degree of n.
 * https://oeis.org/A328385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 728, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   degree_from_factors, factor_u64, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328385: Arithmetic derivative iteration
   For n ≥ 1:
     * If n = 1 return 0
     * If n is prime return 1
     * If n = p^p return n
     * Otherwise iterate x → A003415(x) until either
         – the value is prime – return that prime
         – the degree (maximum prime exponent) differs from that of the
           initial n – return that value
     * If the iteration reaches a fixed point, return that value.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/degree_from_factors.h"

/* compute the arithmetic derivative of a uint64_t number */
static inline uint64_t derivative_uint64(uint64_t n,
                                         const uint64_t *pr,
                                         const uint64_t *ex,
                                         size_t cnt)
{
    uint64_t d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        d += (n / pr[i]) * ex[i];
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    uint64_t cur = (uint64_t)n;

    /* prime test */
    if (is_prime_small(cur) || is_prime_big((seq_t)cur))
        return (seq_t)1;

    /* factor current number */
    uint64_t pr[20], ex[20];
    size_t cnt;
    factor_u64(cur, pr, ex, &cnt);

    /* p^p test */
    if (cnt == 1 && pr[0] == ex[0])
        return (seq_t)cur;

    /* initial degree */
    uint64_t orig_deg = degree_from_factors(ex, cnt);

    while (1) {
        /* arithmetic derivative */
        uint64_t der = derivative_uint64(cur, pr, ex, cnt);

        /* fixed point check */
        if (der == cur)
            return (seq_t)cur;

        /* prime check */
        if (is_prime_small(der) || is_prime_big((seq_t)der))
            return (seq_t)der;

        /* factor derivative for next check */
        factor_u64(der, pr, ex, &cnt);
        uint64_t der_deg = degree_from_factors(ex, cnt);

        /* degree difference check */
        if (der_deg != orig_deg)
            return (seq_t)der;

        cur = der; /* continue with new current value */
    }
}
