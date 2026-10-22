/*
 * A303934  Number of ways to write 2*n as p + 2^k + 5^m with p prime and 2^k
 *          + 5^m squarefree, where k and m are nonnegative integers.
 * https://oeis.org/A303934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implement a brute‑force enumeration of all suitable pairs (k,m):
   a(n) = |{ (p,k,m) : 2*n = p + 2^k + 5^m,
                    p is prime,
                    2^k + 5^m is squarefree }|
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul64.h"

/* ------------------------------------------------------------ */
/*  Basic utilities used only internally                           */
/* ------------------------------------------------------------ */

/* modular multiplication avoiding overflow on 64‑bit integers */

/* modular exponentiation (binary exponentiation) */
static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        e >>= 1;
    }
    return res;
}

/* deterministic Miller‑Rabin for 64‑bit integers.
   Bases 2,3,5,7,11,13,17 are valid for all n < 3·10^12,
   which is well above the values we hit (n <= 2·10^10). */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    const uint64_t small[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1; ++s;
    }

    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t a = small[i];
        if (a >= n) break;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* integer square root (floor) of a uint64 number */
static idx_t isqrt_idx(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* ------------------------------------------------------------ */
/*  Square‑free table, grown lazily as larger n are requested  */
/* ------------------------------------------------------------ */

static bool *squarefree = NULL;
static idx_t squarefree_size = 0;

/* Ensure squarefree[0 .. max_s] is valid */
static void compute_squarefree(idx_t max_s)
{
    idx_t new_size = max_s + 1;
    bool *new_sf = (bool*)malloc(new_size * sizeof(bool));
    if (!new_sf) exit(1);

    for (idx_t i = 0; i < new_size; ++i) new_sf[i] = true;
    if (new_size > 0) new_sf[0] = false;
    if (new_size > 1) new_sf[1] = false;

    idx_t limit = isqrt_idx(max_s);
    bool *is_prime_small = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime_small) exit(1);
    for (idx_t i = 2; i <= limit; ++i) is_prime_small[i] = true;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime_small[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_small[j] = false;

    for (idx_t p = 2; p <= limit; ++p)
        if (is_prime_small[p]) {
            idx_t sq = p * p;
            for (idx_t j = sq; j < new_size; j += sq)
                new_sf[j] = false;
        }

    free(is_prime_small);
    free(squarefree);
    squarefree = new_sf;
    squarefree_size = new_size;
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                      */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below the OEIS offset */

    idx_t two_n = n * 2;
    idx_t max_s = (two_n >= 2) ? two_n - 2 : 0;
    if (max_s < 2) max_s = 2;        /* at least size 3 for indices 0..2 */

    if (squarefree_size <= max_s) compute_squarefree(max_s);

    uint64_t count = 0;

    idx_t pow2_val = 1;
    while (pow2_val <= max_s) {
        idx_t pow5_val = 1;
        while (pow5_val <= max_s) {
            idx_t s = pow2_val + pow5_val;
            if (s >= 2 && s <= max_s && squarefree[s]) {
                idx_t p = two_n - s;
                if (p >= 2 && isPrime64(p)) ++count;
            }
            if (pow5_val > (max_s / 5)) break;
            pow5_val *= 5;
        }
        if (pow2_val > (max_s / 2)) break;
        pow2_val <<= 1;
    }

    return (seq_t)count;
}
