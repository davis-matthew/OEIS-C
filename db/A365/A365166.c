/*
 * A365166  Numbers that are not the sum of a prime number and a fourth power
 *          of a nonnegative integer.
 * https://oeis.org/A365166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365166: Numbers that are not the sum of a prime number and
   a fourth power of a non‑negative integer. */

#include <stdint.h>
#include <stdbool.h>
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

/* Computes floor(n^(1/4)) for an arbitrary‑precision or fixed‑width
   integer value n. The result fits in an idx_t. */
static inline idx_t floor_fourth_root_seq(seq_t n)
{
    seq_t s  = floor_sqrt(n);   /* floor(sqrt(n))   */
    seq_t ss = floor_sqrt(s);   /* floor(sqrt(sqrt(n))) */
    return (idx_t)ss;           /* safe cast; ss is small */
}

/* Returns true iff m can be written as prime + k^4 (k ≥ 0). */
static bool _is_representable(seq_t m)
{
    const seq_t two = 2;
    idx_t kmax = floor_fourth_root_seq(m);
    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t k2 = kseq * kseq;
        seq_t k4 = k2 * k2;
        seq_t p = m - k4;               /* p = m - k^4 */
        if (p < two) continue;          /* p must be at least 2 */
        if (is_prime_big(p))
            return true;
    }
    return false;
}

/* Cached sequence storage.  The array grows by doubling its
   capacity whenever it becomes full. */
static seq_t *cache      = NULL;   /* array of sequence values       */
static size_t cache_cap  = 0;      /* current allocated capacity     */
static size_t cache_len  = 0;      /* number of values stored        */
static idx_t last_checked = 0;    /* last integer examined          */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, but safe */

    /* initial allocation */
    if (!cache)
    {
        cache_cap = 64;
        cache     = mem_new(seq_t, cache_cap);
        if (!cache) return 0;       /* allocation failure (unlikely) */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        ++last_checked;             /* start from 1 on first call   */

        if (!_is_representable(last_checked))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0; /* allocation failure */

                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];             /* 0‑based indexing in the array */
}
