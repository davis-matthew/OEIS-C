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
 * Helpers:   is_prime_big
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
#include "../utils/is_prime_big.h"

static seq_t *cache_a = NULL;
static seq_t *cache_b = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_a = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
        seq_t *new_b = (seq_t *)realloc(cache_b, new_cap * sizeof(seq_t));
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
