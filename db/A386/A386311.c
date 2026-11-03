/*
 * A386311  a(1) = 2, a(n+1) is the largest prime p such that b(n+1) = b(n)*(p
 *          + a(n))/(p - a(n)) is a positive integer, where b(1) = 1.
 * https://oeis.org/A386311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386311:  a(1)=2; for n>1,
   a(n) is the largest prime p such that
   b(n)=b(n-1)*(p + a(n-1))/(p - a(n-1)) is an integer,
   with b(1)=1.
   The function computes a(n) for any n ≥ 1 using memoisation.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *cache_a = NULL;
static seq_t *cache_b = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_a = mem_resize(seq_t, cache_a, new_cap);
        seq_t *new_b = mem_resize(seq_t, cache_b, new_cap);
        if (!new_a || !new_b) abort();
        cache_a = new_a;
        cache_b = new_b;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, safe guard */

    /* initialise cache with first term */
    if (cache_len == 0) {
        ensure_capacity();
        cache_a[0] = (seq_t)2;          /* a(1) */
        cache_b[0] = (seq_t)1;          /* b(1) */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    while (cache_len < n) {
        seq_t a_prev = cache_a[cache_len - 1];
        seq_t b_prev = cache_b[cache_len - 1];

        /* v = 2 * a_prev * b_prev */
        seq_t v = a_prev * b_prev;
        v = v + v;   /* multiply by 2 */

        /* search for smallest divisor t of v that yields prime a_next */
        idx_t t = 1;
        seq_t a_next, b_next;
        for (;;) {
            seq_t t_seq = (seq_t)t;
            if ((v % t_seq) == 0) {
                seq_t p = a_prev + (v / t_seq);   /* candidate prime */
                if (is_prime_big(p)) {
                    a_next = p;
                    /* compute b_next exactly */
                    seq_t numerator = b_prev * (p + a_prev);
                    seq_t denominator = p - a_prev;
                    b_next = numerator / denominator;
                    break;
                }
            }
            ++t;
        }

        ensure_capacity();
        cache_a[cache_len] = a_next;
        cache_b[cache_len] = b_next;
        ++cache_len;
    }

    return cache_a[n - 1];
}
