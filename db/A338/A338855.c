/*
 * A338855  Concatenating all successive absolute differences between two
 *          successive digits of a(n) produce a subchain of a(n).
 * https://oeis.org/A338855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the lexicographically earliest sequence of distinct
 *            positive integers with the property that concatenating the
 *            absolute differences of successive digits of a(n) yields a
 *            contiguous subchain of a(n)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates the sequence term by term,
   storing previously computed values.  Each new term is the
   smallest integer (≥10) not yet used that satisfies the
   property.  The algorithm runs in O(n^2) worst‑case for
   membership checks; here membership is trivial because
   the sequence is strictly increasing, so a simple dynamic
   array suffices.  The property test is performed by
   extracting the decimal digits and checking all possible
   alignments of the difference string.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline seq_t digit_diff(seq_t a, seq_t b)
{
    /* both a and b are digits 0-9 */
    return (a > b) ? (a - b) : (b - a);
}

/* Test whether a positive integer v satisfies the sequence
   property.  v must be at least two digits; smaller values
   always fail. */
static bool satisfies(seq_t v)
{
    if (v < 10) return false;

    /* Count digits and store them. */
    seq_t temp = v;
    int len = 0;
    while (temp > 0) {
        temp /= 10;
        len++;
    }

    uint8_t *d = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!d) exit(1);               /* memory allocation failure */
    temp = v;
    for (int i = len - 1; i >= 0; --i) {
        d[i] = (uint8_t)(temp % 10);
        temp /= 10;
    }

    int diff_len = len - 1;
    for (int start = 0; start + diff_len <= len; ++start) {
        bool ok = true;
        for (int i = 0; i < diff_len; ++i) {
            int diff = (int)digit_diff(d[i], d[i + 1]);
            if (diff != d[start + i]) { ok = false; break; }
        }
        if (ok) { free(d); return true; }
    }

    free(d);
    return false;
}

/* Dynamic array for caching sequence values. */
static seq_t *cache   = NULL;   /* array of computed a(k) */
static idx_t  cache_size = 0;   /* number of values stored */
static idx_t  cache_cap  = 0;   /* allocated capacity */
static seq_t  next_candidate = 10;   /* next integer to test */

/* Ensure that the cache has room for at least n more elements. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void append_term(seq_t val)
{
    ensure_cache_cap(cache_size + 1);
    cache[cache_size++] = val;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, just return 0 */

    /* Generate terms until we reach n. */
    while (cache_size < n) {
        if (satisfies(next_candidate)) {
            append_term(next_candidate);
        }
        next_candidate++;
    }

    return cache[n - 1];
}
