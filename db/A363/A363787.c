/*
 * A363787  Primitive binary Niven numbers: binary Niven numbers (A049445)
 *          that are not twice another binary Niven number.
 * https://oeis.org/A363787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive binary Niven numbers – computed by brute‑force enumeration */
#include <stdbool.h>
#include <stdlib.h>

/* Static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 1;

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Count 1‑bits in a binary representation of x. */
static seq_t popcount_seq(seq_t x)
{
    seq_t w = 0;
    while (x) {
        w += x & 1;
        x >>= 1;
    }
    return w;
}

/* Test whether x is a binary Niven number. */
static bool is_binary_niven(seq_t x)
{
    seq_t w = popcount_seq(x);
    return w > 0 && (x % w == 0);
}

/* Return the nth primitive binary Niven number (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cur = next_candidate;

        if (is_binary_niven(cur)) {
            bool primitive = true;
            if (!(cur & 1)) { /* cur is even */
                seq_t half = cur >> 1;
                if (is_binary_niven(half))
                    primitive = false; /* cur = 2 * (binary Niven) → skip */
            }
            if (primitive) {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cur;
            }
        }

        next_candidate++;
    }

    return cache[n - 1];
}
