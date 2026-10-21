/*
 * A296758  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296758 – Numbers whose base‑15 digits have fewer rises
   than falls.  The function scans the natural numbers, tests the
   rise/fall condition in base 15, and memoises the first n numbers
   that satisfy it. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Helper: test if a number satisfies the rise/fall condition        */
/* ------------------------------------------------------------------ */
static bool _in_a296758(seq_t x)
{
    /* A single‑digit number cannot have a rise or fall. */
    if (x < 15)
        return false;

    /* Store base‑15 digits – LSB first.  The array size can be
       generous; 1024 digits more than cover all numbers we will
       handle in the test range. */
    const int MAX_D = 1024;
    uint8_t digits[MAX_D];
    int len = 0;
    seq_t t = x;

    while (t != 0) {
        digits[len++] = (uint8_t)(t % 15);
        t /= 15;
        if (len == MAX_D)
            break;               /* safety – the number is huge */
    }

    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        uint8_t a = digits[i];     /* higher place */
        uint8_t b = digits[i-1];   /* lower place */
        if (a < b)
            ++rises;
        else if (a > b)
            ++falls;
    }
    return rises < falls;
}

/* ------------------------------------------------------------------ */
/*  Cache for memoisation – two implementations, one for C and one
   for C++ (to allow arbitrary‑precision seq_t that may be a class).   */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> _cache_vec;
static idx_t _next_cand = 1;
#else
static seq_t * _cache = NULL;   /* dynamic array */
static idx_t  _cache_size = 0;   /* number of cached terms */
static idx_t  _cache_cap   = 0;   /* allocated capacity */
static idx_t  _next_cand   = 1;   /* next natural number to test */
#endif

#ifndef __cplusplus
/* C‑only helper to grow the dynamic array.  The cast is needed in C++. */
static void _grow_cache(idx_t need)
{
    if (need <= _cache_cap)
        return;
    idx_t new_cap = (_cache_cap == 0) ? 8 : _cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!newptr)
        exit(1);               /* allocation failed */
    _cache     = newptr;
    _cache_cap = new_cap;
}
#endif

/* ------------------------------------------------------------------ */
/*  Main function – returns the nth term of A296758 (1‑based index).    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* behaviour for n < 1 is irrelevant – return 0 */
    if (n == 0)
        return 0;

#ifdef __cplusplus
    /* Grow vector until it holds at least n terms */
    while (_cache_vec.size() < n) {
        if (_in_a296758(_next_cand))
            _cache_vec.push_back(_next_cand);
        ++_next_cand;
    }
    return _cache_vec[n - 1];
#else
    /* Ensure allocated capacity */
    if (n > _cache_cap)
        _grow_cache(n);

    /* Generate new terms until we have n cached */
    while (_cache_size < n) {
        if (_in_a296758(_next_cand))
            _cache[_cache_size++] = _next_cand;
        ++_next_cand;
    }
    return _cache[n - 1];
#endif
}
