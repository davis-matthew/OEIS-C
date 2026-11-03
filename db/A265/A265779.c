/*
 * A265779  Denominators of lower primes-only best approximates (POBAs) to
 *          sqrt(3); see Comments.
 * https://oeis.org/A265779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th denominator of the lower primes‑only best approximates to
   sqrt(3).  The algorithm maintains the current best approximation p/q
   (with q prime) and for each successive prime q it finds the largest prime
   p ≤ floor(sqrt(3*q*q)).  If p/q is greater than the current best and
   still < sqrt(3), the pair becomes the new best and its denominator is
   recorded.  This method produces the exact OEIS sequence A265779. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* return the next prime > start (prime search in O(gap)).  The function
   works for seq_t values that can be represented as unsigned integers
   within the practical range of the problem. */
static seq_t next_prime_seq(seq_t start)
{
    seq_t cand = start + (seq_t)1;
    while (!is_prime_big(cand))
        cand = cand + (seq_t)1;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for 0, just guard */

    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    static seq_t best_num    = (seq_t)0;   /* best numerator seen so far   */
    static seq_t best_den    = (seq_t)0;   /* best denominator seen so far */
    static seq_t current_q   = (seq_t)0;   /* current prime to test        */

    /* initialise on first call */
    if (cache_len == 0)
    {
        current_q = (seq_t)2;      /* first prime */
    }

    /* generate terms until we have n of them */
    while (cache_len < n)
    {
        seq_t q = current_q;

        /* limit = floor(sqrt(3*q*q)) = largest integer ≤ sqrt(3)*q */
        seq_t sq    = (seq_t)3 * q * q;
        seq_t limit = floor_sqrt(sq);

        /* p = largest prime ≤ limit */
        seq_t p = limit;
        while (!is_prime_big(p))
            p = p - (seq_t)1;

        /* check whether p/q is a new record (greater than current best) */
        bool better = false;
        if (best_den == (seq_t)0)
        {
            better = true;          /* first fraction seen */
        }
        else
        {
            seq_t left  = p * best_den;  /* p / q  > best_num / best_den  */
            seq_t right = best_num * q;
            if (left > right) better = true;
        }

        if (better)
        {
            /* store the new denominator in the cache */
            if (cache_len + (size_t)1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)4;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = q;
            best_num = p;
            best_den = q;
        }

        /* advance to the next prime */
        current_q = next_prime_seq(q);
    }

    return cache[n - (size_t)1];
}
