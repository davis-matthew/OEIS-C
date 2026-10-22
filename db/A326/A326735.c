/*
 * A326735  Numbers which converge to 2 under repeated application of the
 *          powertrain map of A133500.
 * https://oeis.org/A326735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers which converge to 2 under repeated application of the *
 *            powertrain map of A133500. The sequence is defined by iterating
 *            the * map f(n) = ∏ base^exp over each adjacent pair of decimal
 *            digits of n * (if the number of digits is odd, multiply by the
 *            last digit). The term * a(n) is the n‑th po...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* The integer types used by the build */
typedef uint64_t idx_t;          /* index type */

/* Forward declaration of seq_t, supplied by the build. Do not define it. */
extern seq_t dummy; /* for the compiler to know seq_t is a type */

/* ----------  powertrain map  ---------- */
static seq_t powertrain_map(seq_t n)
{
    if (n == (seq_t)0)     /* 0 maps to 0 */
        return (seq_t)0;

    /* Collect decimal digits in reverse order */
    uint8_t digits[50];
    size_t dcnt = 0;
    seq_t temp = n;
    while (temp != (seq_t)0) {
        digits[dcnt++] = (uint8_t)(temp % (seq_t)10);
        temp /= (seq_t)10;
    }

    seq_t result = (seq_t)1;
    /* Process pairs of digits: most‑significant of the pair is the base */
    for (size_t i = 0; i + 1 < dcnt; i += 2) {
        uint8_t base = digits[dcnt - 1 - i];
        uint8_t exp  = digits[dcnt - 1 - (i + 1)];
        seq_t power = (seq_t)1;
        for (uint8_t k = 0; k < exp; ++k)
            power *= (seq_t)base;
        result *= power;
    }
    /* If the number of digits is odd, multiply by the least‑significant
       digit (the first stored in digits[0]) */
    if (dcnt & 1U)
        result *= (seq_t)digits[0];
    return result;
}

/* ----------  test for eventual convergence to 2  ---------- */
static bool reaches_two(seq_t start)
{
    if (start == (seq_t)2)
        return true;

    seq_t slow = start, fast = start;
    for (;;) {
        slow = powertrain_map(slow);
        if (slow == (seq_t)2)
            return true;

        fast = powertrain_map(powertrain_map(fast));
        if (fast == (seq_t)2)
            return true;

        if (slow == fast)     /* cycle detected */
            return false;
        if (slow == (seq_t)0 || fast == (seq_t)0)
            return false;     /* 0 is a trivial cycle not leading to 2 */
    }
}

/* ----------  cache storage  ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static idx_t last_checked = 2;   /* smallest candidate to test */
#else
static seq_t *cache  = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t last_checked = 2;  /* smallest candidate to test */
#endif

static void cache_append(seq_t val)
{
#ifdef __cplusplus
    cache_vec.push_back(val);
#else
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();      /* allocation failure */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
#endif
}

#ifndef __cplusplus
static size_t cache_size()
{
    return cache_len;
}
#endif

/* ----------  the sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset 1: a(1) is the first term */
        return (seq_t)0;

#ifdef __cplusplus
    /* grow the vector until we have n terms (vector index = n-1) */
    while (cache_vec.size() < n) {
        seq_t cand = (seq_t)last_checked;
        if (reaches_two(cand))
            cache_vec.push_back(cand);
        ++last_checked;
    }
    return cache_vec[n - 1];
#else
    /* grow the dynamic array until we have n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)last_checked;
        if (reaches_two(cand))
            cache_append(cand);
        ++last_checked;
    }
    return cache[n - 1];
#endif
}
