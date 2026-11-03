/*
 * A300587  Real part of the n-th Gaussian prime x + i*y, x >= y >= 0, ordered
 *          by norm x^2 + y^2.
 * https://oeis.org/A300587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 926, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9471 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: scan all integers in increasing order, picking those that
 * correspond to a Gaussian prime norm:
 *   • N = 2   → principal Gaussian prime 1+i (real part 1)
 *   • N = p   where p ≡ 1 (mod 4) is a rational prime → prime (a,b) with a ≥ b,
 *             real part a
 *   • N = p² where p ≡ 3 (mod 4) is a rational prime → prime (p,0),
 *             real part p
 * For each such N the real part of the associated Gaussian prime is
 * stored.  The n‑th stored value is returned by a(n).  The list is
 * memoised so any permutation of calls is handled efficiently. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;      /* memoised sequence a(n) */
static size_t cache_len = 0;     /* number of elements stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t  next_candidate = 2;/* next integer to test for a norm */

/* compute the real part of a Gaussian prime whose norm is the prime
 * p (p ≡ 1 (mod 4)).  We search for integers a ≥ b > 0 with a²+b² = p.
 * The search starts from ⌊√p⌋ downwards because the larger component a
 * is desired. */
static seq_t prime_real_part(seq_t p)
{
    idx_t a = (idx_t)floor_sqrt(p);
    for (; a >= 1; --a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        seq_t b2 = p - a2;
        seq_t b  = floor_sqrt(b2);
        if (b * b == b2)                /* found representation a²+b²=p */
            return (seq_t)a;             /* a ≥ b by construction */
    }
    return (seq_t)0; /* should never happen for a valid prime p */
}

/* extend cache so that it contains at least n elements (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        seq_t val;

        /* search for the next admissible norm */
        while (1) {
            if (next_candidate == 2) {       /* special norm 2 */
                val = (seq_t)1;               /* real part of 1+i */
                ++next_candidate;
                break;
            }

            /* case 1: norm is a prime p ≡ 1 (mod 4) */
            if (is_prime_big((seq_t)next_candidate) &&
                (next_candidate % 4 == 1)) {
                val = prime_real_part((seq_t)next_candidate);
                ++next_candidate;
                break;
            }

            /* case 2: norm is the square of a prime p ≡ 3 (mod 4) */
            seq_t sqrtN = floor_sqrt((seq_t)next_candidate);
            if (sqrtN * sqrtN == (seq_t)next_candidate) { /* perfect square? */
                if (is_prime_big(sqrtN) && (sqrtN % 4 == 3)) {
                    val = sqrtN;                /* real part = p */
                    ++next_candidate;
                    break;
                }
            }
            ++next_candidate;                   /* next integer to test */
        }

        /* append val to cache, enlarging if necessary */
        if (cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    ensure_cache(n);                      /* guarantee cache[n-1] exists */
    return cache[n - 1];
}
