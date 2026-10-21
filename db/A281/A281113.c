/*
 * A281113  Number of twice-factorizations of n. Number of ways to choose a
 *          postpositive factorization of each part of a postpositive
 *          factorization of n.
 * https://oeis.org/A281113
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all unordered factorizations of n of the product
 *            of g(part), where g(m) is the number of unordered factorizations
 *            of m. This function implements the definition directly
 * Range:     u8 n <= 191, u16 n <= 9591, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   divisor_count, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code uses a simple memoisation strategy:  g_cache stores g(n) for each
    n, and a_cache stores a(n) once computed.  Because the definition is
    inherently recursive, a straightforward recursion suffices for n up to
    about 1e4.  The implementation is valid in C11 and in C++17; the only
    C++-only feature is the use of unordered_map for auxiliary caching of
    intermediate results in the recursive evaluation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* Forward declaration of helper functions that may be provided by the
   utilities library.  They must be defined elsewhere.  If they are not
   needed for the build, the macros will be ignored. */
#ifndef NO_UTILS
#include "../utils/gcd.h"
#include "../utils/divisor_count.h"
#endif

/* External linkage as required */
seq_t a(idx_t n);

/* --------------------------------------------------------------------------
   Internal helpers
   -------------------------------------------------------------------------- */

/* Simple cache for g(n) = number of postpositive factorizations of n. */
static seq_t *g_cache = NULL;
static idx_t cache_size = 0;

/* Ensure that g_cache is allocated for all indices up to idx.  Newly
   allocated entries are initialised to 0. */
static void ensure_cache(idx_t idx)
{
    if (cache_size > idx)
        return;
    size_t new_cap = (size_t)(idx + 1);   /* make space for 0..idx */
    g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!g_cache)
        abort(); /* out of memory */
    for (idx_t i = cache_size; i <= idx; ++i)
        g_cache[i] = 0;
    cache_size = idx;
}

/* Recursively count the number of unordered factorizations of n.
   count_fact(n, min) assumes that all factors are >= min and are
   non‑decreasing to avoid double counting. */
static seq_t count_fact(idx_t n, idx_t min)
{
    /* trivial factorisation [n] */
    seq_t total = 1;
    for (idx_t d = min; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t rest = n / d;
            if (d <= rest)               /* maintain non‑decreasing order */
                total += count_fact(rest, d);
        }
    }
    return total;
}

/* Return g(n).  The value is cached after the first computation. */
static seq_t get_g(idx_t n)
{
    if (n < 2)
        return 0;
    ensure_cache(n);
    if (g_cache[n] != 0)
        return g_cache[n];
    g_cache[n] = count_fact(n, 2);
    return g_cache[n];
}

/* --------------------------------------------------------------------------
   Recursive evaluation of a(n) with memoisation for the pair (n, min).
   Because the recursion depends on the minimal allowed factor in the
   current step, we cache results keyed by (n, min).
   -------------------------------------------------------------------------- */

#ifdef __cplusplus
/* C++ implementation uses unordered_map for caching */
#include <unordered_map>

/* Key is packed as (n << 32) | min.  n and min are < 2^32 for the
   problem size considered. */
static std::unordered_map<uint64_t, seq_t> series_cache;

static seq_t series_fun(idx_t n, idx_t min)
{
    uint64_t key = ((uint64_t)n << 32) | (uint64_t)min;
    auto it = series_cache.find(key);
    if (it != series_cache.end())
        return it->second;

    /* trivial factorisation [n] */
    seq_t sum = get_g(n);

    for (idx_t d = min; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t rest = n / d;
            if (d <= rest) {
                seq_t sub = series_fun(rest, d);
                sum += get_g(d) * sub;
            }
        }
    }

    series_cache[key] = sum;
    return sum;
}
#else
/* C implementation: simple open‑addressing hash table for (n, min).  Only used
   when the build is C (i.e. seq_t is a builtin integer). */
typedef struct {
    idx_t key;    /* packed as (n<<32)|min */
    seq_t val;
    bool used;
} SeriesEntry;

static SeriesEntry *series_tbl = NULL;
static size_t series_tbl_cap = 0;
static size_t series_tbl_nmemb = 0;

static size_t series_hash(idx_t key)
{
    return (size_t)(key * 11400714819323198485ULL) & (series_tbl_cap - 1);
}

static void series_tbl_rehash(void)
{
    size_t new_cap = series_tbl_cap ? series_tbl_cap * 2 : 64;
    SeriesEntry *new_tbl = (SeriesEntry *)calloc(new_cap, sizeof(*new_tbl));
    if (!new_tbl)
        abort();
    size_t i;
    for (i = 0; i < series_tbl_cap; ++i) {
        if (series_tbl[i].used) {
            size_t h = series_hash(series_tbl[i].key);
            while (new_tbl[h].used)
                h = (h + 1) & (new_cap - 1);
            new_tbl[h] = series_tbl[i];
        }
    }
    free(series_tbl);
    series_tbl = new_tbl;
    series_tbl_cap = new_cap;
}

static seq_t series_fun(idx_t n, idx_t min)
{
    if (!series_tbl_cap)
        series_tbl_rehash();
    idx_t key = (n << 32) | min;
    size_t h = series_hash(key);
    while (series_tbl[h].used && series_tbl[h].key != key)
        h = (h + 1) & (series_tbl_cap - 1);

    if (series_tbl[h].used)
        return series_tbl[h].val;

    seq_t sum = get_g(n);
    for (idx_t d = min; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t rest = n / d;
            if (d <= rest) {
                seq_t sub = series_fun(rest, d);
                sum += get_g(d) * sub;
            }
        }
    }

    if (series_tbl_nmemb * 4 >= series_tbl_cap * 3)
        series_tbl_rehash();
    h = series_hash(key);
    while (series_tbl[h].used)
        h = (h + 1) & (series_tbl_cap - 1);
    series_tbl[h].used = true;
    series_tbl[h].key = key;
    series_tbl[h].val = sum;
    ++series_tbl_nmemb;
    return sum;
}
#endif

/* --------------------------------------------------------------------------
   Public interface
   -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;           /* undefined below offset */
    /* Ensure that g_cache is at least large enough for all intermediate
       values that will be referenced during the recursion that follows. */
    ensure_cache(n);
    return series_fun(n, 2);
}
