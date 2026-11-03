/*
 * A265772  Numerators of lower primes-only best approximates (POBAs) to
 *          sqrt(2); see Comments.
 * https://oeis.org/A265772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 14, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function generates the OEIS sequence A265772: the numerators of the
 * lower primes‑only best approximations to √2.  The algorithm processes
 * successive prime denominators q, computes the largest prime numerator
 * p ≤ ⌊q·√2⌋, and records the pair (p,q) whenever the fraction p/q is
 * strictly larger than all previously found fractions.  Those recorded
 * numerators form the sequence.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* memoisation storage -----------------------------------------------------*/
static seq_t *cache      = NULL;  /* cache of numerators */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */

/* tracking best fraction found so far -------------------------------------*/
static seq_t best_num = 0; /* numerator of best fraction seen so far */
static seq_t best_den = 1; /* denominator of best fraction seen so far */

/* index of next prime denominator to examine -----------------------------*/
static idx_t next_prime_idx = 1;  /* prime #1 is 2 */

/* Compare two fractions p/q and best_num/best_den without overflow.        */
static bool better_fraction(seq_t p, seq_t q)
{
    seq_t lhs = seq_mul(p, best_den);      /* p * best_den     */
    seq_t rhs = seq_mul(best_num, q);      /* best_num * q   */
    return seq_cmp(&lhs, &rhs) > 0;        /* lhs > rhs ? */
}

/* ------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range; harmless */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute terms until we have at least n cached values. */
    while (cache_len < n) {
        /* ----- fetch next prime denominator ---------------------------- */
        uint32_t prime_q = get_prime(next_prime_idx++);
        seq_t   q = (seq_t)prime_q;

        /* ----- find largest prime p ≤ floor(q·√2) --------------------- */
        seq_t two_qq = seq_mul((seq_t)2, seq_mul(q, q));
        seq_t L      = floor_sqrt(two_qq);        /* L = ⌊q·√2⌋           */
        seq_t p = L;
        while (!is_prime_big(p)) --p;

        /* ----- record if new fraction is better ------------------------ */
        if (better_fraction(p, q)) {
            best_num = p;
            best_den = q;

            /* grow cache when necessary */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];
}
