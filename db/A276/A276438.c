/*
 * A276438  a(n) = (A274647(n)-A274647(n-1)) / n.
 * https://oeis.org/A276438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 34, i16 n <= 7724, i32 n <= 9591, i64 n <= 9591,
 *            i128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iteratively constructing A274647(n) greedily.
   In A274647 the accumulated value A(0)=0 is kept.
   For each i≥1 we choose the smallest k≥1 such that either
   A(i‑1)+i·k or A(i‑1)–i·k is a non‑negative integer that has not yet
   appeared (and zero is considered already used).  If both options are
   available we take the smaller resultant value.
   The returned a(i) is that chosen k with a minus sign when the minus
   option was used.  The sequence is memoised so that calls in any
   order are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Global static storage */
static seq_t *cache = NULL;        /* memoised a(i) values, 1‑based */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static idx_t last_computed = 0;    /* last n for which a(n) was computed */

static uint8_t *used = NULL;       /* bitmap of used A(i) values */
static size_t used_bytes = 0;      /* number of bytes allocated in used */
static seq_t current_val = 0;      /* A(n‑1), starts from 0 */

/* Ensure the cache has space for at least 'needed' items.  'needed' is
   the number of indices (including index 0) that must exist.  */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the bitmap has space for value 'v'.  The bitmap stores one bit
   per non‑negative integer.  */
static void ensure_used_size(seq_t v)
{
    if (v < 0) return;
    size_t idx  = (size_t)v;
    size_t byte = idx >> 3;
    if (byte >= used_bytes) {
        size_t new_bytes = byte + 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_bytes);
        memset(tmp + used_bytes, 0, new_bytes - used_bytes);
        used = tmp;
        used_bytes = new_bytes;
    }
}

/* Mark 'v' as used (set its bit to 1). */
static void used_set(seq_t v)
{
    if (v < 0) return;
    ensure_used_size(v);
    size_t idx  = (size_t)v;
    used[idx >> 3] |= (uint8_t)(1U << (idx & 7));
}

/* Test whether 'v' has already been used.  Returns 1 if used, 0 otherwise. */
static int used_contains(seq_t v)
{
    if (v < 0) return 1; /* negative numbers are treated as used */
    size_t idx  = (size_t)v;
    size_t byte = idx >> 3;
    if (byte >= used_bytes) return 0;
    return (used[byte] >> (idx & 7)) & 1;
}

/* Main function as required. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n==0, but safe to return 0 */

    /* Ensure cache has space for up to n (0..n). */
    ensure_cache_size((size_t)n + 1);

    /* On the very first call, mark zero as used (already in A(0)). */
    if (last_computed == 0) {
        used_set(0);
    }

    /* Compute all missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t inc = (seq_t)i;           /* i cast to seq_t for multiplication */
        seq_t chosen_val = 0;           /* resulting A(i) */
        seq_t chosen_k   = 0;           /* magnitude of k used */
        int   sign       = 0;           /* +1 for addition, -1 for subtraction */

        /* Try k = 1, 2, 3, ... until a valid unique value is found. */
        for (seq_t k = 1; ; ++k) {
            seq_t prod   = inc * k;
            seq_t plus   = current_val + prod;
            int   plus_ok = !used_contains(plus);

            int   minus_ok = 0;
            seq_t minus;
            if (current_val >= prod) {
                minus = current_val - prod;
                minus_ok = !used_contains(minus);
            }

            if (plus_ok || minus_ok) {
                if (plus_ok && minus_ok) {
                    if (minus <= plus) {
                        chosen_val = minus;
                        chosen_k   = k;
                        sign       = -1;
                    } else {
                        chosen_val = plus;
                        chosen_k   = k;
                        sign       = +1;
                    }
                } else if (plus_ok) {
                    chosen_val = plus;
                    chosen_k   = k;
                    sign       = +1;
                } else { /* minus_ok */
                    chosen_val = minus;
                    chosen_k   = k;
                    sign       = -1;
                }
                break;
            }
        }

        cache[i] = (sign > 0) ? chosen_k : -chosen_k;
        current_val = chosen_val;
        used_set(chosen_val);
    }

    last_computed = n;
    return cache[n];
}
