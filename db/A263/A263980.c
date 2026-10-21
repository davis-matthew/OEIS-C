/*
 * A263980  Least prime of the form a^2 + b^2 with a > k and b > k, for some
 *          number k.
 * https://oeis.org/A263980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263980: Least prime of the form a²+b² with a>k and b>k,
   with the sequence listing distinct members as k increases. */

/*  The elements are produced by scanning primes in ascending order.
    For each prime p we compute the minimum of the two numbers in any
    representation p = a² + b² (a ≤ b).  A prime is the candidate for
    every k with k < a.  The sequence consists of the first distinct
    prime that appears when we step through k = 0, 1, 2, … .
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Utility prototypes from the build system */
#include "../utils/is_prime_big.h"

/* ---------- arithmetic helpers ---------- */

/* Modular exponentiation for 64‑bit numbers */
static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = (__uint128_t)res * a % mod;
        a = (__uint128_t)a * a % mod;
        e >>= 1;
    }
    return res;
}

/* Tonelli–Shanks: compute a square root of n modulo prime p.
   Returns 0 if no square root exists.  p must be an odd prime. */
static uint64_t tonelli_shanks(uint64_t n, uint64_t p) {
    if (n == 0) return 0;
    if (p == 2) return n;
    if (mod_pow64(n, (p - 1) >> 1, p) != 1) return 0;
    if (p % 4 == 3) return mod_pow64(n, (p + 1) >> 2, p);

    uint64_t q = p - 1;
    int s = 0;
    while ((q & 1) == 0) {
        q >>= 1;
        ++s;
    }

    uint64_t z = 2;
    while (mod_pow64(z, (p - 1) >> 1, p) != p - 1) ++z;
    uint64_t c = mod_pow64(z, q, p);
    uint64_t r = mod_pow64(n, (q + 1) >> 1, p);
    uint64_t t = mod_pow64(n, q, p);
    int m = s;

    while (t != 1) {
        uint64_t tt = t;
        int i = 0;
        do {
            tt = (__uint128_t)tt * tt % p;
            ++i;
        } while (tt != 1 && i < m);
        uint64_t b = mod_pow64(c, 1ULL << (m - i - 1), p);
        r = (__uint128_t)r * b % p;
        c = (__uint128_t)b * b % p;
        t = (__uint128_t)t * c % p;
        m = i;
    }
    return r;
}

/* Integer square root (floor).  Assumes n fits in 64 bits. */
static uint64_t isqrt_uint64(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Cornacchia's algorithm.
   Given p and t such that t² ≡ –1 (mod p),
   returns a,b with p = a² + b² (a ≤ b).
   Returns true on success. */
static bool cornacchia(uint64_t p, uint64_t t,
                       uint64_t *a_out, uint64_t *b_out) {
    uint64_t a = p, b = t;
    while (b * b > p) {
        uint64_t tmp = b;
        b = a % b;
        a = tmp;
    }
    uint64_t r = p - b * b;
    uint64_t s = isqrt_uint64(r);
    if (s * s != r) return false;
    *a_out = b;
    *b_out = s;
    return true;
}

/* Minimum of the two numbers in the representation of prime p.
   Returns 1 for p = 2, 0 if p cannot be written as a²+b². */
static uint64_t min_ab_of(uint64_t p) {
    if (p == 2) return 1;
    if ((p & 3) != 1) return 0;          /* not of the form 1 mod 4 */
    uint64_t t = tonelli_shanks(p - 1, p);  /* sqrt(–1) mod p */
    if (!t) return 0;
    uint64_t a, b;
    if (!cornacchia(p, t, &a, &b)) return 0;
    return a < b ? a : b;
}

/* ---------- prime generator ---------- */

static uint64_t _prime_candidate = 2;
static bool _prime_started = false;

static void _next_prime(uint64_t *prime_out) {
    if (!_prime_started) {
        *prime_out = 2;
        _prime_started = true;
        _prime_candidate = 3;
        return;
    }
    uint64_t c = _prime_candidate;
    for (;;) {
        if (is_prime_big((seq_t)c)) {
            *prime_out = c;
            _prime_candidate = c + 2;     /* skip even numbers */
            return;
        }
        c += 2;
    }
}

/* ---------- memoised sequence storage ---------- */

static seq_t *seq_cache  = NULL;   /* 1‑based array, seq_cache[1] = a(1) */
static size_t seq_cap   = 0;       /* allocated capacity, >= seq_len + 1 */
static size_t seq_len   = 0;       /* number of terms cached */
static uint64_t current_k = 0;     /* current k to search for */

/* Ensure seq_cache has capacity for at least 'need' elements (1‑based). */
static void _ensure_capacity(size_t need) {
    if (need <= seq_cap) return;
    size_t old_cap = seq_cap;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cap = new_cap;
    memset((char *)seq_cache + (old_cap + 1) * sizeof(seq_t), 0,
           (new_cap - old_cap) * sizeof(seq_t));
}

/* Extend the cache until the nth term is available. */
static void _ensure_cache(idx_t n) {
    if (n <= seq_len) return;   /* already computed */
    _ensure_capacity((size_t)n);
    while (seq_len < (size_t)n) {
        uint64_t p;
        _next_prime(&p);
        uint64_t m = min_ab_of(p);
        if (m > current_k) {
            /* Store this new prime as the next distinct member. */
            seq_cache[++seq_len] = (seq_t)p;
            current_k = m;          /* next k skips to m */
        }
    }
}

/* ---------- required function ---------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but safe */
    _ensure_cache(n);
    return seq_cache[(size_t)n];
}
