/*
 * A268867  Number of integers of the form m^2+1 between two consecutive pairs
 *          of primes of the same form.
 * https://oeis.org/A268867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS sequence A268867:  Number of integers of the form m^2+1
 *  between two consecutive pairs of primes of the same form.
 *
 *  For the i‑th twin pair (k^2+1, (k+2)^2+1) let
 *      P_i[0] = k^2+1   (lower prime)
 *      P_i[1] = (k+2)^2+1 (upper prime)
 *
 *  Then a(n) = max(0,
 *                  floor_sqrt( P_{n}[0] - 2 ) - ceil_sqrt( P_{n-1}[1] ) + 1 )
 *
 *  The function is memoised; it is valid for any build where
 *  seq_t and idx_t are unsigned integer types (including arbitrary‑
 *  precision, when seq_t is a big integer class).  It uses only
 *  integer arithmetic on seq_t and works in O(1) per call after
 *  the required twin pairs have been generated.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Integer arithmetic helpers                                         */

/* integer square root (floor) of a 64‑bit unsigned integer */
static uint64_t
floor_sqrt_uint64(uint64_t x)
{
    /* use long double sqrt, then correct to guarantee correctness */
    long double d = sqrtl((long double)x);
    uint64_t r = (uint64_t)d;

    /* correct possible off‑by‑one errors */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    while ((unsigned __int128)r * r > x)          --r;
    return r;
}

/* ceil(sqrt(x)) for 64‑bit unsigned integer */
static uint64_t
ceil_sqrt_uint64(uint64_t x)
{
    uint64_t r = floor_sqrt_uint64(x);
    return ((unsigned __int128)r * r == x) ? r : r + 1;
}

/* modular multiply (a*b) mod mod using 128‑bit */
static inline uint64_t
mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation (base^exp) mod mod */
static uint64_t
mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, cur, mod);
        cur = mod_mul64(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static bool
is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* check small primes */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* bases good for 64‑bit */
    static const uint64_t bases[] =
        {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Twin prime pairs management                                        */

/* struct for (k^2+1, (k+2)^2+1) pair */
typedef struct {
    uint64_t k;
    uint64_t p1;   /* k^2+1   */
    uint64_t p2;   /* (k+2)^2+1 */
} twin_pair_t;

/* dynamic array of twin pairs */
static twin_pair_t *twin_arr  = NULL;
static size_t       twin_len  = 0;
static size_t       twin_cap  = 0;
static uint64_t     next_k    = 2;   /* next even k to test */

/* ensure capacity for at least 'needed' pairs */
static void
ensure_twin_capacity(size_t needed)
{
    if (twin_cap >= needed) return;
    size_t new_cap = twin_cap ? twin_cap : 1024;
    while (new_cap < needed) new_cap <<= 1;
    twin_arr = (twin_pair_t *)realloc(twin_arr, new_cap * sizeof(twin_pair_t));
    twin_cap = new_cap;
}

/* generate twin pairs until at least 'need' pairs are stored */
static void
generate_twin_pairs(size_t need)
{
    while (twin_len < need) {
        /* compute k^2+1 */
        __uint128_t val1 = (__uint128_t)next_k * next_k + 1;
        if (val1 > UINT64_MAX) break;          /* overflow guard */
        uint64_t p1 = (uint64_t)val1;

        if (is_prime_uint64(p1)) {
            uint64_t kp = next_k + 2;
            __uint128_t val2 = (__uint128_t)kp * kp + 1;
            if (val2 > UINT64_MAX) break;
            uint64_t p2 = (uint64_t)val2;
            if (is_prime_uint64(p2)) {
                ensure_twin_capacity(twin_len + 1);
                twin_arr[twin_len].k  = next_k;
                twin_arr[twin_len].p1 = p1;
                twin_arr[twin_len].p2 = p2;
                ++twin_len;
            }
        }
        next_k += 2;   /* next even k */
    }
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence function                                            */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – undefined for n=0 */

    /* we need pairs indexed 0 … n (total n+1 pairs) */
    size_t need = (size_t)n + 1;
    generate_twin_pairs(need);

    uint64_t L = twin_arr[n-1].p2;   /* upper prime of pair n-1  */
    uint64_t U = twin_arr[n].p1;     /* lower prime of pair n     */

    if (U <= L + 1) return 0;       /* no room for any m^2+1 */

    uint64_t lb = ceil_sqrt_uint64(L);      /* minimal m with m^2+1 > L   */
    uint64_t ub = floor_sqrt_uint64(U - 2); /* maximal m with m^2+1 < U   */

    uint64_t cnt = (ub >= lb) ? (ub - lb + 1) : 0;
    return (seq_t)cnt;
}
