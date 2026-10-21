/*
 * A289286  Starting with a(1) = 1, a(n) = smallest nonnegative integer not
 *          yet in the sequence such that the last digit of a(n-1) plus the
 *          first digit of a(n) is equal to 8. The digit 0 is not allowed.
 * https://oeis.org/A289286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   has_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289286: a(1)=1; for n>1, a(n) is the smallest unused non‑negative integer
   without digit 0 such that last digit of a(n‑1) plus first digit of a(n) equals 8.
   The sequence terminates after 57 terms. */
/* This implementation memoises the sequence; it works for all supported
   seq_t types (integer or arbitrary‑precision) and for any order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_nonzero_digits.h"

/* Helpers --------------------------------------------------------------- */

static inline int first_digit(seq_t x) {
    if (x == (seq_t)0) return 0;
    while (x >= (seq_t)10) x = x / (seq_t)10;
    return (int)x;
}

/* Memoisation storage ----------------------------------------------- */

static unsigned char *used   = NULL;   /* bool flag: 1 if number already in sequence */
static idx_t          used_cap = 0;

static seq_t *cache   = NULL;   /* array of sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure used array can index at least idx (and zero‑extend) */
static void ensure_used(idx_t idx) {
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap <= (size_t)idx) new_cap = (size_t)idx + 1;
    unsigned char *tmp = (unsigned char*)realloc(used, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used   = tmp;
    used_cap = new_cap;
}

/* Ensure cache can hold at least len elements */
static void ensure_cache(size_t len) {
    if (len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < len) new_cap = len;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache  = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the sequence */
static void add_value(seq_t val) {
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
    ensure_used((idx_t)val);
    used[(idx_t)val] = 1;
}

/* Main function -------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* undefined for n<1 */

    /* Generate the sequence up to the requested index (or until it terminates). */
    while (cache_len < n) {
        if (cache_len == 0) {          /* the first term is 1 */
            add_value((seq_t)1);
            continue;
        }

        seq_t   prev      = cache[cache_len - 1];
        int     last_digit = (int)(prev % (seq_t)10);
        int     req_first  = 8 - last_digit;

        if (req_first <= 0 || req_first > 9) {
            /* No further term can satisfy the requirement. */
            break;
        }

        /* Find the smallest unused candidate with the required first digit. */
        seq_t candidate = 0;
        for (;;) {
            idx_t ci = (idx_t)candidate;
            ensure_used(ci);
            if (!used[ci] && has_nonzero_digits(candidate) && first_digit(candidate) == req_first)
                break;
            ++candidate;
        }
        add_value(candidate);
    }

    if (n <= cache_len) return cache[n - 1];
    /* n is beyond the finite length of the sequence */
    return (seq_t)0;
}
