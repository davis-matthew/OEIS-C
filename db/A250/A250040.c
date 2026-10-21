/*
 * A250040  Numbers n such that m = floor(n/10) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 3183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250040 – Right‑Truncated Coprime numbers in base 10.
 * The function a(idx_t n) generates the n‑th term (1‑based) by
 * scanning successive integers and accepting those with
 *   gcd(k, floor(k/10)) == 1
 * and floor(k/10) == 0 or already present in the sequence.
 * All found terms are cached; subsequent calls reuse the cache
 * regardless of call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Dynamic array of seq_t values.  The allocation strategy is
 * different for C++ (array of objects) and C (realloc of POD). */
static seq_t *terms      = NULL;   /* array of values */
static size_t terms_size = 0;      /* number of stored terms */
static size_t terms_cap  = 0;      /* capacity of the array */
static seq_t  next_candidate = 1;  /* next integer to test */

/* Ensure the array has at least `needed` elements: grow if necessary. */
static void ensure_capacity(size_t needed)
{
    if (terms_cap >= needed) return;

    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_buf = new seq_t[new_cap];
    if (terms) {
        for (size_t i = 0; i < terms_size; ++i) new_buf[i] = terms[i];
        delete[] terms;
    }
    terms = new_buf;
#else
    seq_t *new_buf = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_buf;
#endif
    terms_cap = new_cap;
}

/* Append a new value to the cached sequence. */
static void append_value(seq_t val)
{
    ensure_capacity(terms_size + 1);
    terms[terms_size++] = val;
}

/* Binary search: does `val` already exist in `terms`?  The array is
 * sorted in ascending order. */
static bool contains(seq_t val)
{
    size_t lo = 0, hi = terms_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (terms[mid] == val) return true;
        if (terms[mid] < val)      lo = mid + 1;
        else                       hi = mid;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; 0 is never requested */

    /* Generate terms until we have at least `n` of them. */
    while (terms_size < (size_t)n) {
        seq_t cand = next_candidate;
        seq_t m    = cand / (seq_t)10U;   /* floor division */

        if (gcd(cand, m) == (seq_t)1) {
            if (m == (seq_t)0 || contains(m))
                append_value(cand);
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
