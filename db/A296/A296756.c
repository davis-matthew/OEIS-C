/*
 * A296756  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296756: numbers whose base‑15 digits have an
   equal number of rises and falls.  The implementation
   scans the natural numbers sequentially, tests each one,
   and caches the found terms for fast repeated queries.
   No closed form is known, so a linear search with caching
   is used. */

#ifdef __cplusplus
#  include <vector>
#  include <cstdlib>
#  include <cstdbool>
#else
#  include <stdlib.h>
#  include <stdbool.h>
#endif

/* SCALAR for converting small constants to the seq_t type */
#define SEQ_CONST(x) ((seq_t)(x))

/* Function that returns true iff candidate has equal rises
   and falls in its base‑15 representation. */
static bool base15_rise_fall_equal(seq_t candidate)
{
    /* Quick return for the single‑digit case */
    if (candidate == SEQ_CONST(0) || candidate == SEQ_CONST(1))
        return true;

    uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = candidate;

    /* Extract digits in little‑endian order */
    while (tmp != SEQ_CONST(0)) {
        digits[len++] = (uint8_t)(tmp % SEQ_CONST(15));
        tmp = tmp / SEQ_CONST(15);
    }

    if (len <= 1)
        return true;

    size_t rises = 0, falls = 0;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i-1] < digits[i])
            ++rises;
        else if (digits[i-1] > digits[i])
            ++falls;
    }
    return rises == falls;
}

/* ---------------- C++‑specific implementation ---------------- */

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static seq_t last_checked = SEQ_CONST(0);

/* Ensure that at least n terms are cached */
static void ensure_terms_cpp(idx_t n)
{
    while (cache_vec.size() < n) {
        seq_t candidate = last_checked + SEQ_CONST(1);
        last_checked = candidate;
        if (base15_rise_fall_equal(candidate))
            cache_vec.push_back(candidate);
    }
}
#endif

/* ---------------- C‑specific implementation ---------------- */

#ifndef __cplusplus
static seq_t *cache = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;
static seq_t last_checked = SEQ_CONST(0);

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();
    cache       = new_ptr;
    cache_cap   = new_cap;
}

static void ensure_terms_c(idx_t n)
{
    while (cache_sz < n) {
        seq_t candidate = last_checked + SEQ_CONST(1);
        last_checked = candidate;
        if (base15_rise_fall_equal(candidate)) {
            if (cache_sz >= cache_cap)
                grow_cache();
            cache[cache_sz++] = candidate;
        }
    }
}
#endif

/* ----------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                     /* sequence is 1‑based */

#ifdef __cplusplus
    ensure_terms_cpp(n);
    return cache_vec[n - 1];
#else
    ensure_terms_c(n);
    return cache[n - 1];
#endif
}
