/*
 * A341537  a(n) is the number of digits from the end of the concatenation of
 *          all previous terms where n last appears. If n has not previously
 *          appeared then a(n) = n.
 * https://oeis.org/A341537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   last_occurrence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A341537:  a(n) = distance from the end of the
   concatenation of all previous terms to the last occurrence of the
   decimal representation of n.  If n has not appeared before, a(n)=n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/last_occurrence.h"

/* Static state used by the algorithm. */
static seq_t          *cache      = NULL;
static bool           *cache_seen = NULL;
static size_t          cache_cap   = 0;     /* allocated size of cache */
static idx_t           cache_next  = 0;     /* next index to compute */

static unsigned char   *digits     = NULL;
static size_t          digits_cap   = 0;    /* allocated size of digits buffer */
static size_t          digits_len   = 0;    /* number of digits currently stored */

/* ---------------------------------------------------------------------- */
/* Helper: convert a seq_t value to an array of its decimal digits
   (most significant first).  *len is set to the number of digits. */
static void seq_to_digits(seq_t x, unsigned char *buf, int *len)
{
    if (x == (seq_t)0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    seq_t temp = x;
    const seq_t ten = (seq_t)10;
    int i = 0;
    while (temp != (seq_t)0) {
        seq_t d = temp % ten;
        buf[i++] = (unsigned char)d;
        temp = temp / ten;
    }
    /* reverse to normal order */
    for (int j = 0; j < i / 2; ++j) {
        unsigned char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    *len = i;
}

/* Ensure the cache arrays can hold at least index `needed`. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
    bool  *new_seen  = (bool *)realloc(cache_seen, new_cap * sizeof(*cache_seen));
    if (!new_cache || !new_seen) abort();           /* allocation failure */
    /* Initialize newly allocated entries */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    memset(new_seen + cache_cap, 0, (new_cap - cache_cap) * sizeof(bool));
    cache = new_cache;
    cache_seen = new_seen;
    cache_cap = new_cap;
}

/* Ensure the digit buffer can hold at least `needed` elements. */
static void ensure_digits_cap(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *new_digits = (unsigned char*)realloc(digits, new_cap);
    if (!new_digits) abort();
    digits = new_digits;
    digits_cap = new_cap;
}

/* Append the decimal representation of `val` to the global digit
   buffer `digits`, updating the running length. */
static void append_value(seq_t val)
{
    unsigned char buf[128];
    int blen;
    seq_to_digits(val, buf, &blen);

    if (digits_len + (size_t)blen > digits_cap)
        ensure_digits_cap(digits_len + (size_t)blen);

    for (int i = 0; i < blen; ++i)
        digits[digits_len++] = buf[i];
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initial allocation if this is the first call. */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(*cache));
        cache_seen = (bool*)realloc(NULL, cache_cap * sizeof(*cache_seen));
        memset(cache_seen, 0, cache_cap * sizeof(bool));
    }

    /* Allocate more space if necessary. */
    if ((size_t)n >= cache_cap) ensure_cache_cap((size_t)n + 1);

    /* Compute missing terms up to n. */
    if (!cache_seen[n]) {
        for (idx_t k = cache_next; k <= n; ++k) {
            /* Convert k to decimal pattern. */
            unsigned char pat[128];
            int plen;
            seq_to_digits((seq_t)k, pat, &plen);

            int start = last_occurrence(digits, (int)digits_len,
                                       pat, plen);

            seq_t ak;
            if (start < 0) {
                ak = (seq_t)k;
            } else {
                ak = (seq_t)(digits_len - (size_t)start);
            }

            cache[k] = ak;
            cache_seen[k] = true;

            append_value(ak);
        }
        cache_next = n + 1;
    }

    return cache[n];
}
