/*
 * A300587  Real part of the n-th Gaussian prime x + i*y, x >= y >= 0, ordered
 *          by norm x^2 + y^2.
 * https://oeis.org/A300587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 926, u32 n <= 3321, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, grow_seq, is_prime_big
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
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

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
