/*
 * A324282  Lexicographically earliest sequence of different terms starting
 *          with a(1) = 10 such that the n-th digit of the sequence, placed
 *          after a(n) and then concatenated, produces a term with a parity
 *          opposed to the parity of a(n).
 * https://oeis.org/A324282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 229, u16 n <= 9577, u32 n <= 9577, u64 n <= 9577,
 *            u128 n <= 9577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A324282 – lexicographically earliest permutation of positive integers
 *  starting with a(1)=10 such that the n‑th digit of the concatenated
 *  digit sequence, concatenated to a(n), yields a number whose parity
 *  is opposite to that of a(n).  The function generates terms on demand,
 *  caching previously computed terms and the digit stream.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#include <set>
#endif

/*--------------------------------------------------------------------
 * Internal state – shared between C and C++ builds
 *--------------------------------------------------------------------*/
static bool initialized = false;

/*  Next candidates of required parity; updated after each insertion.  */
static seq_t next_even = 2;
static seq_t next_odd  = 1;

/*--------------------------------------------------------------------
 * C implementation
 *--------------------------------------------------------------------*/
#ifndef __cplusplus
/*  Array of already returned terms  */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/*  Boolean map of used numbers  */
static bool *used      = NULL;
static size_t used_cap = 0;

/*  Buffer of all digits in the concatenated sequence  */
static unsigned char *digits = NULL;
static size_t digits_cap     = 0;
static size_t digits_len     = 0;

/*  Ensure the used array can index at least idx (0‑based).  */
static void ensure_used_cap(size_t idx)
{
    if (idx < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= idx) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    if (used)  /* new slots are initially false */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    else
        for (size_t i = 0; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/*  Ensure the cache can hold at least n elements  */
static void ensure_cache_cap(idx_t n)
{
    if ((idx_t)cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while ((idx_t)new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/*  Ensure the digits buffer can hold at least len elements  */
static void ensure_digits_cap(size_t len)
{
    if (digits_cap >= len) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 16;
    while (new_cap < len) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(digits, new_cap * sizeof(unsigned char));
    if (!tmp) exit(EXIT_FAILURE);
    digits = tmp;
    digits_cap = new_cap;
}

/*  Append the decimal digits of x (most‑to‑least) to the digits buffer.  */
static void append_digits(seq_t x)
{
    seq_t t = x;
    unsigned char buf[64];
    size_t pos = 0;
    do {
        buf[pos++] = (unsigned char)(t % 10);
        t /= 10;
    } while (t > 0);
    ensure_digits_cap(digits_len + pos);
    for (size_t i = pos; i > 0; --i)
        digits[digits_len++] = buf[i - 1];
}

/*  Return the smallest unused number of the given parity,
 *  mark it as used, and advance the next candidate pointer.  */
static seq_t next_candidate(bool need_even)
{
    seq_t cand = need_even ? next_even : next_odd;
    for (;;) {
        size_t idx = (size_t)cand;
        ensure_used_cap(idx);
        if (!used[idx]) break;
        cand += 2;
    }
    used[(size_t)cand] = true;
    if (need_even) {
        next_even = cand + 2;
        while ((size_t)next_even < used_cap && used[(size_t)next_even])
            next_even += 2;
    } else {
        next_odd = cand + 2;
        while ((size_t)next_odd < used_cap && used[(size_t)next_odd])
            next_odd += 2;
    }
    return cand;
}
#else /* __cplusplus */
/*  C++ implementation – uses STL containers to store state.  */
static std::vector<seq_t> cache_vec;
static std::set<seq_t> used_set;
static std::vector<unsigned char> digits_vec;

/*  Append the decimal digits of x to digits_vec.  */
static void append_digits(const seq_t x)
{
    seq_t t = x;
    std::vector<unsigned char> rev;
    do {
        rev.push_back(static_cast<unsigned char>(t % 10));
        t /= 10;
    } while (t > 0);
    for (auto it = rev.rbegin(); it != rev.rend(); ++it)
        digits_vec.push_back(*it);
}

/*  Return the smallest unused number of the given parity,
 *  mark it as used, and advance the next candidate pointer.  */
static seq_t next_candidate(bool need_even)
{
    seq_t cand = need_even ? next_even : next_odd;
    for (;;) {
        if (used_set.find(cand) == used_set.end()) break;
        cand += 2;
    }
    used_set.insert(cand);
    if (need_even) {
        next_even = cand + 2;
        while (used_set.find(next_even) != used_set.end())
            next_even += 2;
    } else {
        next_odd = cand + 2;
        while (used_set.find(next_odd) != used_set.end())
            next_odd += 2;
    }
    return cand;
}
#endif /* __cplusplus */

/*--------------------------------------------------------------------
 *  Main public function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!initialized) {
        initialized = true;
#ifndef __cplusplus
        /* Allocate minimal arrays */
        cache = NULL;      cache_cap = 0;      cache_len = 0;
        used  = NULL;      used_cap  = 0;
        digits = NULL;     digits_cap = 0;     digits_len = 0;

        /* Insert a(1)=10, mark as used and append its digits. */
        ensure_cache_cap(1);
        cache[0] = (seq_t)10;
        cache_len = 1;
        ensure_used_cap(10);
        used[10] = true;
        append_digits((seq_t)10);
#else
        cache_vec.clear();
        used_set.clear();
        digits_vec.clear();

        cache_vec.push_back((seq_t)10);
        used_set.insert((seq_t)10);
        append_digits((seq_t)10);
#endif
        /* next_even/next_odd already set to 2/1, no need to adjust here */
    }

    /* If the requested term is already cached, return it. */
#ifndef __cplusplus
    if ((idx_t)cache_len >= n)
        return cache[n - 1];
#else
    if (cache_vec.size() >= (size_t)n)
        return cache_vec[n - 1];
#endif

    /* Extend cache up to n. */
#ifndef __cplusplus
    ensure_cache_cap(n);
    for (idx_t k = (idx_t)cache_len + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1) {
            val = (seq_t)10;          /* already inserted during init */
        } else {
            unsigned char d = digits[k - 1];    /* k-th digit (0‑based) */
            bool need_even = (d & 1);           /* d odd → need even */
            val = next_candidate(need_even);
        }
        cache[k - 1] = val;
        cache_len = (size_t)k;
        append_digits(val);
    }
    return cache[n - 1];
#else
    for (size_t k = cache_vec.size() + 1; k <= (size_t)n; ++k) {
        seq_t val;
        if (k == 1) {
            val = (seq_t)10;          /* already in vector */
        } else {
            unsigned char d = digits_vec[k - 1];    /* k‑th digit */
            bool need_even = (d & 1);
            val = next_candidate(need_even);
        }
        cache_vec.push_back(val);
        append_digits(val);
    }
    return cache_vec[n - 1];
#endif
}
