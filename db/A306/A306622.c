/*
 * A306622  a(1) = 1, for n >= 2, a(n) = Sum_{i=1..floor(log_2(n))} a(n-i).
 * https://oeis.org/A306622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = Σ_{i=1}^{⌊log₂(n)⌋} a(n-i)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 37, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
/* The following type aliases are provided by the build system: */
/* - idx_t  : unsigned 64‑bit integer (e.g. uint64_t) */
/* - seq_t  : unsigned integer of 8, 16, 32, 64 or 128 bits, */
/*            or an arbitrary‑precision integer class.          */

/* local helper: floor( log₂(v) ) for unsigned 64‑bit integers */
static inline idx_t _floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >> 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

/* Persistent memoised recurrence */
seq_t a(idx_t n)
{
    if (n == 0u)          /* defined only for n ≥ 1, return 0 for n = 0 */
        return (seq_t)0;

    /* --- Persistent cache: cache[0] = a(1), cache[1] = a(2), … --- */
    static seq_t *cache = NULL;
    static size_t  len   = 0;   /* number of terms currently stored   */
    static size_t  cap   = 0;   /* allocated capacity (number of slots) */

    /* Ensure the cache can hold terms up to n (1‑based).            */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        /* Cast is required for C++ compatibility. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof *tmp);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any terms that have not yet been cached. */
    for (size_t i = len; i < (size_t)n; ++i) {
        seq_t value;
        if (i == 0) {                     /* a(1) */
            value = (seq_t)1;
        } else {
            idx_t idx = (idx_t)(i + 1);    /* real n = i+1, 1‑based            */
            idx_t lim = _floor_log2_uint64(idx);
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= lim; ++j)
                sum += cache[i - j];
            value = sum;
        }
        cache[i] = value;
    }

    len = (size_t)n;          /* now cache[0..n-1] contain a(1)…a(n)   */
    return cache[(size_t)n - 1];
}
