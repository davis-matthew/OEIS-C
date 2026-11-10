/*
 * A276947  First differences of A256450: a(n) = A256450(n) - A256450(n-1).
 * https://oeis.org/A276947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A256450(n) - A256450(n-1), where A256450 (indexed from 0)
 *            lists numbers with a digit 1 in factorial base; computed by
 *            search with a cache
 * Range:     u8 n <= 2195, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Validated: standard, pass, 136 terms through n = 4993 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A276947_has_one(seq_t m)
{
    if (m == (seq_t)0)
        return false;
    seq_t t = m;
    seq_t base = 2;
    while (t > (seq_t)0) {
        if (t % base == (seq_t)1)
            return true;
        t = t / base;
        ++base;
    }
    return false;
}

static seq_t *A276947_cache = NULL;
static size_t A276947_cap = 0;
static size_t A276947_len = 0;
static seq_t A276947_next = 2;

static void A276947_ensure(size_t n)
{
    if (A276947_cache == NULL) {
        A276947_cap = 64;
        A276947_cache = mem_new(seq_t, A276947_cap);
        if (!A276947_cache)
            abort();
        A276947_cache[0] = (seq_t)1;
        A276947_len = 1;
    }
    while (A276947_len <= n) {
        while (!A276947_has_one(A276947_next))
            ++A276947_next;
        if (A276947_len >= A276947_cap) {
            A276947_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, A276947_cache, A276947_cap);
            if (!tmp)
                abort();
            A276947_cache = tmp;
        }
        A276947_cache[A276947_len++] = A276947_next;
        ++A276947_next;
    }
}

seq_t a(idx_t n)
{
    A276947_ensure((size_t)n);
    return A276947_cache[(size_t)n] - A276947_cache[(size_t)n - 1];
}
