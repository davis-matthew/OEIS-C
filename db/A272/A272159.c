/*
 * A272159  Numbers k such that abs(8*k^2 - 488*k + 7243) is prime.
 * https://oeis.org/A272159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod512, pow_mod512
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k, where k is the n‑th integer (starting at k = 0) such
   that either k <= 61 or |8*k^2 - 488*k + 7243| is prime.  The offset of
   the sequence is 1 (so a(1)=0, a(2)=1, …).  The function caches the
   found terms in a static array and uses a deterministic Miller–Rabin
   test for 64‑bit integers to decide primality of the quadratic
   expression. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_mod512.h"
#include "../utils/pow_mod512.h"

/* ---------------------------------------------------------------------- */
/* 64‑bit deterministic Miller–Rabin primality test                       */

static bool is_prime_uint64(uint64_t n)
{
    /* small primes and trivial divisibility tests */
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                            19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod512(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod512(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Test whether a given k satisfies the definition of the sequence.      */
/* ---------------------------------------------------------------------- */
static inline bool k_is_in_sequence(seq_t k)
{
    /* All integers 0 .. 61 are always in the sequence. */
    if (k <= 61) return true;

    /* Compute |8*k^2 - 488*k + 7243| in 128‑bit to avoid overflow. */
    __int128 kk   = (__int128)k;
    __int128 val  = 8 * kk * kk - 488 * kk + 7243;
    if (val < 0)  val = -val;
    uint64_t expr = (uint64_t)val;          /* fits in 64 bits for all k used. */

    return is_prime_uint64(expr);
}

/* ---------------------------------------------------------------------- */
/* Main sequence accessor.  Caches results in a static array.             */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of found terms                   */
    static size_t  cap    = 0;      /* allocated capacity                      */
    static idx_t   count  = 0;      /* number of terms stored                   */
    static idx_t   next   = 0;      /* next candidate k to test                 */

    /* Ensure we have at least n terms cached. */
    while (count < n) {
        if (k_is_in_sequence((seq_t)next)) {
            /* Grow the cache if necessary. */
            if (count >= cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure: return 0 as fallback */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = (seq_t)next;
        }
        ++next;
    }

    return cache[n - 1];
}
