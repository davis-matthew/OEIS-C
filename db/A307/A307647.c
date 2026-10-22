/*
 * A307647  Numbers that are the sum of a prime number and a positive cube.
 * https://oeis.org/A307647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A307647 : Numbers that are the sum of a prime number and a positive cube.
    This implementation generates the sequence lazily and caches results.
    It uses a deterministic Miller–Rabin primality test for 64‑bit
    integers, which is sufficient because the terms of this sequence
    quickly fit within 64 bits for any index up to at least 10 000.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ------------------------------------------------------------------ */
/*  Deterministic Miller–Rabin for 64‑bit integers (idx_t is assumed
    to be the 64‑bit unsigned type used for indices).                 */

static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t power  = base % mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, power, mod);
        power = mul_mod64(power, power, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    if (n % 5 == 0)           return n == 5;
    if (n % 7 == 0)           return n == 7;

    /* write n-1 = d * 2^s  with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for n < 2^64 */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence generator with caching                                       */

static seq_t *seq_cache = NULL;     /* cached sequence terms           */
static size_t seq_len  = 0;         /* number of cached terms          */
static size_t seq_cap  = 0;         /* capacity of seq_cache           */

static idx_t *cubes     = NULL;    /* list of cubes j³,  j ≥ 1        */
static size_t cube_len  = 0;        /* number of cubes stored          */
static size_t cube_cap  = 0;        /* capacity of cubes array         */

static void grow_seq_cache(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

static void grow_cubes(size_t min_cap)
{
    if (cube_cap >= min_cap) return;
    size_t new_cap = cube_cap ? cube_cap << 1 : 32;
    if (new_cap < min_cap) new_cap = min_cap;
    idx_t *tmp = (idx_t *)realloc(cubes, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    cubes = tmp;
    cube_cap = new_cap;
}

/* Test whether x can be expressed as prime + j³ (j ≥ 1). */
static bool is_sum_prime_cube(seq_t x)
{
    if (x < 3) return false;           /* smallest sum is 2 + 1 */
    idx_t value = (idx_t)x;

    /* Ensure cubes array contains all cubes ≤ value-2 */
    while (true) {
        idx_t next = cube_len + 1;
        idx_t next_cube = (idx_t)next * next * next;
        if (next_cube > value - 2) break;
        grow_cubes(cube_len + 1);
        cubes[cube_len++] = next_cube;
    }

    /* Check each cube */
    for (size_t i = 0; i < cube_len; ++i) {
        idx_t c = cubes[i];
        idx_t p = value - c;
        if (p < 2) continue;
        if (is_prime_u64((uint64_t)p)) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                            */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but returns 0 */

    static idx_t next_candidate = 1;   /* next integer to test   */

    while (seq_len < n) {
        idx_t cand = next_candidate++;
        if (is_sum_prime_cube((seq_t)cand)) {
            grow_seq_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }
    return seq_cache[n - 1];
}
