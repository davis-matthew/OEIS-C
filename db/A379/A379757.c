/*
 * A379757  a(n) = a(n-1) + 1 with two exceptions: if a(n-1) is prime, a(n) =
 *          a(n-2) + a(n-1), or if a(n-1) is a power, a(n) = a(n-1) / (root
 *          factor), with initial three terms are 0, 1, 2.
 * https://oeis.org/A379757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=2 a(n)=a(n‑1)+1 if a(n‑1) is prime,
 *            a(n)=a(n‑2)+a(n‑1) if a(n‑1) is a perfect power,
 *            a(n)=a(n‑1)/root
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Local helpers -------------------------------------------------------- */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/pow2.h"
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

/* Test whether mid^e == limit.  Emit true only if exact equality holds. */
static bool power_eq(seq_t mid, idx_t e, seq_t limit)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < e; ++i) {
        if (mid != 0 && prod > limit / mid) return false;
        prod *= mid;
    }
    return prod == limit;
}

/* Test whether mid^e <= limit without overflow.
   limit is guaranteed to fit in seq_t. */
static bool power_leq(seq_t mid, idx_t e, seq_t limit)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < e; ++i) {
        if (mid != 0 && prod > limit / mid) return false;
        prod *= mid;
    }
    return prod <= limit;
}

/* Check whether x is a perfect power.  If so store one possible base
   in *base_out and return true; otherwise return false.  We only
   search exponents 2 … 64 – sufficient for all reported terms. */
static bool is_perfect_power(seq_t x, seq_t *base_out)
{
    if (x < 4) return false;            /* 0,1,2,3 are not perfect powers */

    unsigned bits = bit_length(x);    /* bit‑length of x */
    for (idx_t e = 2; e <= 64; ++e) {
        unsigned high_bits = (bits + e - 1) / e;   /* ceil(bits/e) */
        seq_t low  = 2;
        seq_t high = pow2(high_bits);              /* 2^high_bits */
        while (low <= high) {
            seq_t mid = (low + high) / 2;
            if (power_leq(mid, e, x)) {
                if (power_eq(mid, e, x)) {
                    *base_out = mid;
                    return true;
                }
                low = mid + 1;
            } else {
                if (mid == 0) break;  /* safety */
                high = mid - 1;
            }
        }
    }
    return false;
}

/* Main sequence --------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1 – return 0 safely */

    static seq_t *cache = NULL;   /* 1‑based indices stored at cache[n‑1] */
    static idx_t cached_max = 0;  /* largest n computed so far */
    static size_t cap = 0;        /* allocated array size in entries */

    /* Initialise the cache for the first three terms */
    if (cache == NULL) {
        cap = 3;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();      /* allocation failure – terminate */
        cache[0] = (seq_t)0;      /* a(1) */
        cache[1] = (seq_t)1;      /* a(2) */
        cache[2] = (seq_t)2;      /* a(3) */
        cached_max = 3;
    }

    /* Grow the cache when a larger index is requested */
    if (n > cap) {
        size_t new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();        /* allocation failure – terminate */
        cache = tmp;
        cap = new_cap;
    }

    /* If we already have the term, return it directly */
    if (n <= cached_max) return cache[n - 1];

    /* Compute missing terms one by one, using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* i corresponds to a(i) */
        seq_t prev   = cache[i - 2];  /* a(i‑1) */
        seq_t prevprev = cache[i - 3];/* a(i‑2) */
        seq_t next;

        if (is_prime_big(prev)) {
            next = prevprev + prev;
        } else {
            seq_t base;
            if (is_perfect_power(prev, &base)) {
                next = prev / base;
            } else {
                next = prev + 1;
            }
        }
        cache[i - 1] = next;
    }

    cached_max = n;
    return cache[n - 1];
}
