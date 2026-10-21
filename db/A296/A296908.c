/*
 * A296908  Numbers whose base-60 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296908: the nth natural number whose base‑60 representation
 *  has fewer pits than peaks.
 *
 *  A pit at index i satisfies d(i‑1) > d(i) < d(i+1);
 *  a peak satisfies d(i‑1) < d(i) > d(i+1).
 *  The sequence lists numbers in ascending order that satisfy
 *  (#peaks) > (#pits).  The offset is 1; a(1) is the first term.
 *
 *  The implementation builds the sequence on demand and memoises
 *  all already‑generated terms to serve any future calls efficiently.
 *
 *  The method is O(n) in the value of the sought term: it simply
 *  scans natural numbers, tests the pit/peak condition and stores the
 *  qualifying numbers.  Because the density of qualifying numbers is
 *  about one half, generating 10 000 terms requires examining
 *  at most roughly 20 000 integers – well within the time limit.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: test whether a base‑60 number has more peaks than pits.   */
/* ------------------------------------------------------------------ */
static bool has_more_peaks(seq_t v)
{
    /* Extract base‑60 digits (least significant first). */
    uint8_t dig[20];
    int len = 0;

    seq_t t = v;
    if (t == 0) {
        dig[len++] = 0;
    } else {
        while (t != 0) {
            dig[len++] = (uint8_t)(t % (seq_t)60);
            t /= (seq_t)60;
        }
    }

    /* Count peaks and pits. */
    int peaks = 0, pits = 0;
    for (int i = 1; i + 1 < len; ++i) {
        uint8_t a = dig[i-1];
        uint8_t b = dig[i];
        uint8_t c = dig[i+1];
        if (a < b && b > c) {
            ++peaks;
        } else if (a > b && b < c) {
            ++pits;
        }
    }
    return peaks > pits;
}

/* ------------------------------------------------------------------ */
/* Memoised generation of the sequence.                               */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;   /* dynamic array of known terms */
static size_t seq_cap   = 0;       /* capacity of seq_cache */
static size_t seq_len   = 0;       /* number of stored terms        */
static seq_t last_checked = 0;    /* last integer examined         */

/* Ensure the cache has capacity for at least 'need' more elements. */
static void grow_cache(size_t need)
{
    size_t new_cap = seq_cap > 0 ? seq_cap : 16;
    while (new_cap < seq_len + need) {
        new_cap <<= 1;
    }
    seq_t *new_arr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* In a contest or tightly restricted environment this would
         * be an error.  For simplicity we just exit here. */
        abort();
    }
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* Append a value to the cache, expanding if necessary. */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap) {
        grow_cache(1);
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined, return 0 as a fallback */
    }

    /* Generate terms until we have at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (has_more_peaks(last_checked)) {
            cache_append(last_checked);
        }
    }
    return seq_cache[n-1];
}
