/*
 * A296709  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth positive integer whose base‑9 digits have an
 *            equal * number of rises and falls (equal rises and falls). This
 *            property is * equivalent to the signed count of digit
 *            comparisons (rise=+1, fall=-1, * equal=0) summing to zero. The
 *            sequence is generated lazily and cached * so that consec...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9534 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool balanced_rises_falls(seq_t n)
{
    /* return true iff the base‑9 representation of n has equal rises and
     * falls.  The function builds the digit list least‑significant first
     * and then counts adjacent differences. */
    int digits[32];
    int len = 0;
    seq_t tmp = n;

    if (tmp == 0) /* 0 has no rises or falls; not needed in the sequence */
        return true;

    while (tmp > 0) {
        digits[len++] = (int)(tmp % 9);
        tmp /= 9;
    }

    int rise = 0, fall = 0;
    for (int i = 1; i < len; ++i) {
        int diff = digits[i] - digits[i - 1];
        if (diff > 0)
            ++rise;
        else if (diff < 0)
            ++fall;
    }
    return rise == fall;
}

static seq_t *cache = NULL;   /* array of generated terms */
static size_t cached_size = 0;/* number of terms stored */
static size_t cached_cap = 0; /* allocated capacity */
static seq_t next_candidate = 1; /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();
    cache = tmp;
    cached_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based: a(1) is the first term. */
    if (n == 0)
        return 0; /* undefined; behaviour not needed */
    /* Generate terms until we have at least n terms cached. */
    while (cached_size < n) {
        if (balanced_rises_falls(next_candidate)) {
            ensure_cache_capacity(cached_size + 1);
            cache[cached_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
