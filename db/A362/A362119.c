/*
 * A362119  Concatenate the base-6 strings for 1,2,...,n.
 * https://oeis.org/A362119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Integer formed by concatenating the base‑6
 *            representations * of 1,2,…,n, interpreting the resulting
 *            sequence of decimal digits as a * decimal integer. No value is
 *            pre‑computed; the result is built incrementally * from previous
 *            terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 312 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Convert a non‑negative integer `num` to its base‑6 representation
 * and interpret that representation as a decimal integer.
 * len_out receives the number of decimal digits produced. */
static seq_t base6_to_decimal(idx_t num, size_t *len_out)
{
    /* Maximum length for 64‑bit number is < 54 base‑6 digits,
     * which is far below the stack capacity. */
    uint8_t rev[64];
    size_t len = 0;
    idx_t tmp = num;
    do {
        rev[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    } while (tmp != 0);

    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        /* digits are stored in reverse order; process from most
         * significant to least. */
        val = val * (seq_t)10 + rev[len - 1 - i];
    }
    if (len_out) *len_out = len;
    return val;
}

static seq_t *cache = NULL;          /* 0‑indexed storage for a(i) */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of terms already computed */

/* Ensure that the cache can hold at least `need` terms.  Capacity grows
 * geometrically to keep reallocations infrequent. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_mem = mem_resize(seq_t, cache, new_cap);
    if (!new_mem) abort();   /* Out of memory – terminate */
    cache = new_mem;
    cache_cap = new_cap;
}

/* Main sequence function.  It is thread‑unsafe but satisfies the
 * requirements: accepts any order of calls, memoizes results, and
 * runs in O(n) time for the requested n. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 – return 0 */

    /* Return already‑computed term if available. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Ensure storage is available for the requested term. */
    ensure_cache_capacity((size_t)n);

    /* Start from the last known value (or 0 if none). */
    seq_t current = (cache_len > 0) ? cache[cache_len - 1] : (seq_t)0;

    /* Build terms from (cache_len+1) up to n. */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        size_t len;
        seq_t part = base6_to_decimal(i, &len);          /* decimal value of i in base‑6 */
        seq_t mult = pow10(len);                        /* 10^len */
        current = current * mult + part;                /* append the part */
        cache[i - 1] = current;                         /* store a(i) */
    }

    cache_len = (size_t)n;     /* update number of computed terms */
    return current;
}
