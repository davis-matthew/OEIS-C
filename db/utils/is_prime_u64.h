/* is_prime_u64: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_U64_H
#define OEISC_UTILS_IS_PRIME_U64_H
#include <stdbool.h>
#include <stdint.h>

/* Deterministic Miller-Rabin: the first 12 prime bases decide every n below
   3.3e24, so all of uint64_t (it used to be trial division, too slow past
   about 1e12). */
static uint64_t is_prime_u64_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static bool is_prime_u64(uint64_t n)
{
    static const uint64_t P[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; ++i)
        if (n % P[i] == 0) return n == P[i];
    if (n < 1369) return true;                       /* 37^2 */
    uint64_t d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        uint64_t x = 1, b = P[i], e = d;
        while (e) {
            if (e & 1) x = is_prime_u64_mulmod(x, b, n);
            b = is_prime_u64_mulmod(b, b, n);
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (int r = 1; r < s; ++r) {
            x = is_prime_u64_mulmod(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return false;
    }
    return true;
}

#endif
