/*
 * A354359  Dirichlet inverse of A124859.
 * https://oeis.org/A354359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Dirichlet inverse of A124859, multiplicative. For a prime
 *            power p^k: A124859(p^k) = 2 * Π_{j=1}^{k-1} odd_prime_j a(p^k) =
 *            - Σ_{i=1}^{k} A124859(p^i) * a(p^{k-i})
 * Range:     i8 n <= 31, i16 n <= 127, i32 n <= 1023, i64 n <= 9927,
 *            i128 n <= 9927, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

static const uint32_t ODD_PRIMES[] = {
    3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U, 31U,
    37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U, 73U,
    79U, 83U, 89U, 97U, 101U, 103U, 107U, 109U, 113U,
    127U, 131U, 137U, 139U, 149U, 151U, 157U, 163U,
    167U, 173U, 179U, 181U, 191U, 193U, 197U, 199U,
    211U, 223U, 227U, 229U, 233U, 239U, 241U, 251U,
    257U, 263U, 269U, 271U, 277U, 281U, 283U, 293U,
    307U
};

/* Compute a(p^e) for a prime power.  e ≤ 63 for 64‑bit n. */
static seq_t a_prime_power(uint64_t e)
{
    if (e == 0) return (seq_t)1;          /* not used for n>1 */

    seq_t f[64], a[64];
    f[1] = (seq_t)2;                      /* A124859(p^1) */
    a[0] = (seq_t)1;                      /* a(p^0) = 1 */
    a[1] = -(seq_t)2;                     /* a(p^1) = -f[1] */

    if (e == 1) return a[1];

    /* Pre‑compute A124859 values up to e. */
    for (uint32_t k = 2; k <= e; ++k)
        f[k] = f[k - 1] * (seq_t)ODD_PRIMES[k - 2];

    /* Compute Dirichlet inverse values up to e. */
    for (uint32_t k = 2; k <= e; ++k) {
        seq_t sum = 0;
        for (uint32_t i = 1; i <= k; ++i)
            sum += f[i] * a[k - i];
        a[k] = -sum;
    }
    return a[e];
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    const size_t MAX_FACTS = 64;
    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        result *= a_prime_power((uint64_t)exps[i]);

    return result;
}
