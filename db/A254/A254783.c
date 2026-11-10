/*
 * A254783  Numbers k such that A033493(k)/k is an integer.
 * https://oeis.org/A254783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   search over k = 1, 2, 3, ...: k is a term when A033493(k) == 0
 *            (mod k), where A033493(k) is the sum of the Collatz trajectory
 *            of k. Speed-up: A033493(k) = (sum of trajectory values until it
 *            first drops below k) + A033493(that value), using a memo table
 *            of earlier values
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A254783_CACHE_MAX ((size_t)1 << 23)

static uint64_t *A254783_cache = 0;   /* A033493(i) for i < cache_len */
static size_t A254783_cache_len = 0;
static size_t A254783_cache_cap = 0;

static uint64_t *A254783_terms = 0;
static size_t A254783_cnt = 0;
static size_t A254783_cap = 0;
static uint64_t A254783_next = 1;

static unsigned __int128 A254783_traj_sum(uint64_t k)
{
    unsigned __int128 sum = 0;
    uint64_t cur = k;
    while (cur >= (uint64_t)A254783_cache_len) {
        sum += cur;
        if (cur & 1)
            cur = 3 * cur + 1;
        else
            cur >>= 1;
    }
    sum += A254783_cache[cur];
    return sum;
}

static void A254783_init(void)
{
    if (A254783_cache)
        return;
    A254783_cache_cap = 1024;
    A254783_cache = (uint64_t *)malloc(A254783_cache_cap * sizeof(uint64_t));
    A254783_cache[0] = 0;
    A254783_cache[1] = 1;
    A254783_cache_len = 2;
}

seq_t a(idx_t n)
{
    A254783_init();
    while (A254783_cnt < (size_t)n) {
        uint64_t k = A254783_next++;
        unsigned __int128 s = A254783_traj_sum(k);
        if (k == (uint64_t)A254783_cache_len && A254783_cache_len < A254783_CACHE_MAX) {
            if (A254783_cache_len == A254783_cache_cap) {
                size_t nc = A254783_cache_cap * 2;
                if (nc > A254783_CACHE_MAX)
                    nc = A254783_CACHE_MAX;
                A254783_cache = (uint64_t *)realloc(A254783_cache, nc * sizeof(uint64_t));
                A254783_cache_cap = nc;
            }
            A254783_cache[A254783_cache_len++] = (uint64_t)s;
        }
        if (s % k == 0) {
            if (A254783_cnt == A254783_cap) {
                size_t nc = A254783_cap ? A254783_cap * 2 : 64;
                A254783_terms = (uint64_t *)realloc(A254783_terms, nc * sizeof(uint64_t));
                A254783_cap = nc;
            }
            A254783_terms[A254783_cnt++] = k;
        }
    }
    return (seq_t)A254783_terms[n - 1];
}
