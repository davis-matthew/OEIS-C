/*
 * A373652  Composite numbers k for which g = gcd(f(i*c), k) = 1 or k for all
 *          i in the range 1 <= i <= c, where f(x) = Product_{j=1..c} x+j and
 *          c = floor(k^(1/4)).
 * https://oeis.org/A373652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   gcd, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373652: composite numbers k such that
 * let c = ⌊k^(1/4)⌋.  For every i = 1..c,
 *   g = gcd(∏_{j=1..c}(i*c+j), k)
 * is either 1 or k.
 * The implementation performs an on‑demand search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Test whether a given composite k satisfies the required property. */
static bool ok_k(seq_t k)
{
    /* Reject primes */
    if (is_prime_big(k))
        return false;

    /* Compute c = floor(k^(1/4)) = floor(sqrt(floor_sqrt(k))). */
    seq_t s1 = floor_sqrt(k);
    seq_t c  = floor_sqrt(s1);
    if (c == (seq_t)0)
        return false;

    /* For cast safety, ensure c fits into idx_t for the loops. */
    if (c > (seq_t)((idx_t)-1))
        return false;
    idx_t c_idx = (idx_t)c;

    for (idx_t i = 1; i <= c_idx; ++i)
    {
        seq_t base = (seq_t)i * c;
        seq_t prod = 1;

        for (idx_t j = 1; j <= c_idx; ++j)
        {
            seq_t term = base + (seq_t)j;
            prod = (prod * term) % k;   /* product modulo k */
        }

        seq_t g = gcd(prod, k);
        if (g != (seq_t)1 && g != k)
            return false;
    }
    return true;
}

/* Global cache for found sequence terms. */
static seq_t *terms    = NULL;
static size_t term_cnt = 0;
static size_t term_cap = 0;
static seq_t next_k = 4;          /* start checking after 3 */

/* Ensure at least n terms have been computed and cached. */
static void need_terms(idx_t n)
{
    while (term_cnt < n)
    {
        seq_t k = next_k++;

        if (!ok_k(k))
            continue;

        if (term_cnt == term_cap)
        {
            term_cap = (term_cap == 0 ? 16 : term_cap * 2);
            /* cast needed for C++ */
            seq_t *tmp = mem_resize(seq_t, terms, term_cap);
            if (!tmp)
                return;          /* allocation failed – abort further work */
            terms = tmp;
        }
        terms[term_cnt++] = k;
    }
}

/* Public function required by the platform. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined behaviour for n < 1 */
    need_terms(n);
    return terms[n - 1];
}
