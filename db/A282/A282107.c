/*
 * A282107  Numbers n with k digits in base x (MSD(n)_x=d_k, LSD(n)_x=d_1)
 *          such that, chosen one of their digits in position d_k < j < d_1,
 *          is Sum_{i=j+1..k}{(i-j)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x =
 *          2.
 * https://oeis.org/A282107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 8840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A282107 (base‑2 fulcrum numbers).
 * Computation by brute‑force: iterate natural numbers and check the
 * weighted‑sum fulcrum condition.  The sequence is memoized in a
 * dynamically grown static array, so subsequent calls with arbitrary
 * indices are answered in O(1) after the initial precomputation. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the discovered sequence values */
static seq_t *cache = 0;          /* array of values */
static size_t cache_size = 0;     /* number of stored values */
static size_t cache_cap   = 0;    /* capacity of the array */
static uint64_t next_candidate = 1; /* next natural number to test */

/* helper: ensure the cache can hold at least `need` elements */
static void ensure_cache_capacity(size_t need, size_t *cap, void **ptr, size_t elem)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    *ptr = realloc(*ptr, new_cap * elem);
    *cap = new_cap;
}

/* Test whether `n` (as uint64_t) satisfies the fulcrum property in base 2. */
static bool has_fulcrum(uint64_t n)
{
    if (n < 4) return false;                 /* must have at least 3 bits */
    /* number of bits */
    uint64_t nn = n;
    /* builtin_clzll is available both in C and C++ */
    unsigned L = 64 - __builtin_clzll(nn);    /* L >= 3 */
    /* prefix sums of counts and weighted counts */
    uint64_t pCount[65] = {0};
    uint64_t pW[65] = {0};
    for (unsigned i = 1; i <= L; ++i)
    {
        uint64_t d = (nn >> (i-1)) & 1ULL;
        pCount[i] = pCount[i-1] + d;
        pW[i]     = pW[i-1] + d * (uint64_t)i;
    }
    uint64_t totalCount = pCount[L];
    uint64_t totalW     = pW[L];
    /* try all possible fulcrum positions j (2 .. L-1) */
    for (unsigned j = 2; j < L; ++j)
    {
        uint64_t left  = (uint64_t)j * pCount[j-1] - pW[j-1];
        uint64_t right = (totalW - pW[j]) - (uint64_t)j * (totalCount - pCount[j]);
        if (left == right) return true;
    }
    return false;
}

/* Expand the cache until it contains at least `target` terms (target>=1). */
static void extend_cache(idx_t target)
{
    while (cache_size < (size_t)target)
    {
        if (has_fulcrum(next_candidate))
        {
            if (cache_size == cache_cap)
                ensure_cache_capacity(cache_size+1, &cache_cap,
                                      (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Public interface required by the solver. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */
    if (cache_size < (size_t)n) extend_cache(n);
    return cache[n-1];
}
