/*
 * A285688  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of three primes.
 * https://oeis.org/A285688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
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

/* a(1) = 2;
   a(n+1) = the smallest prime > a(n) such that the difference a(n+1)-a(n)
   is the product of exactly three (not necessarily distinct) primes. */

seq_t a(idx_t n)
{
    /* Cache of computed terms.  Indices are 1‑based. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated slots in cache      */
    static size_t computed = 0;/* number of terms already in cache */

    /* initialise cache with the first term */
    if (computed == 0) {
        cap = 1;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)2;
        computed = 1;
    }

    /* grow the cache if we need to compute more terms */
    while (computed < (size_t)n) {
        seq_t cur = cache[computed - 1];
        seq_t cand = cur + (seq_t)1;
        seq_t next;

        /* find the next term */
        for (;;) {
            if (!is_prime_big(cand)) {
                ++cand;
                continue;
            }
            seq_t diff = cand - cur;
            seq_t pf = bigomega_seq(diff);
            if (pf == (seq_t)3) {
                next = cand;
                break;
            }
            ++cand;
        }

        /* ensure capacity for the new term */
        if (computed >= cap) {
            grow_seq(&cache, &cap, computed + 1);
        }

        cache[computed] = next;
        ++computed;
    }

    return cache[n - 1];
}
