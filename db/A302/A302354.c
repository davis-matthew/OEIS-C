/*
 * A302354  Expansion of (Sum_{i>=1} x^prime(i))*(Sum_{j>=0} x^(j^3)).
 * https://oeis.org/A302354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as the sum of a prime and
 *            a non‑negative cube
 * Range:     u8 n <= 117, u16 n <= 117, u32 n <= 117, u64 n <= 117,
 *            u128 n <= 117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the largest j such that j^3 <= n (floor of cube root). */
static inline idx_t floor_cuberoot(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Find an upper bound for the cube root. */
    while ((unsigned __int128)high * high * high <= n) high <<= 1;
    /* Binary search for the exact value. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 m3 = (unsigned __int128)mid * mid * mid;
        if (m3 <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Simple deterministic primality test for idx_t values. */
static inline bool is_prime_local(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i <= x / i; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_j = floor_cuberoot(n);
    for (idx_t j = 0; j <= max_j; ++j) {
        unsigned __int128 cube128 = (unsigned __int128)j * j * j;
        idx_t cube = (idx_t)cube128;               /* j^3 <= n, safe cast */
        if (cube > n) break;
        idx_t remaining = n - cube;                /* remaining >= 0 */
        if (remaining >= 2 && is_prime_local(remaining))
            ++count;
    }
    return count;
}
