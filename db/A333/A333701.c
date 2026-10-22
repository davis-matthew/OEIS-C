/*
 * A333701  Number of ways to write n as the sum of two positive integers with
 *          the same number of distinct prime factors.
 * https://oeis.org/A333701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [omega(i) = omega(n-i)]
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(x) is the number of distinct prime factors of x. */
#include "../utils/smallest_prime_factor.h"

/* Compute omega (number of distinct prime factors) for a single integer. */
static uint8_t _omega(uint64_t n)
{
    if (n <= 1) return 0;
    uint8_t cnt = 0;
    uint64_t last = 0;
    while (n > 1) {
        uint64_t p = smallest_prime_factor(n);
        if (p != last) {
            cnt++;
            last = p;
        }
        while (n % p == 0) n /= p;
    }
    return cnt;
}

/* Cache of omega values up to a certain maximum index. */
static uint8_t *omega_cache = NULL;
static idx_t omega_max = 0;

/* Extend the cache so that positions 0..new_max are valid. */
static void ensure_omega(idx_t new_max)
{
    if (new_max <= omega_max) return;
    uint8_t *tmp = (uint8_t *)realloc(omega_cache, (size_t)(new_max + 1) * sizeof(uint8_t));
    if (!tmp) return;                /* fail silently – values 0 will be treated as valid */
    for (idx_t i = omega_max + 1; i <= new_max; ++i)
        tmp[i] = _omega((uint64_t)i);
    omega_cache = tmp;
    omega_max = new_max;
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* value outside the offset is irrelevant */
    ensure_omega(n);
    seq_t result = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (omega_cache[i] == omega_cache[n - i]) result += 1;
    }
    return result;
}
