/*
 * A373345  Irregular triangle read by rows where row n lists (in decreasing
 *          order) the elements of the Schreier set encoded by A371176(n).
 * https://oeis.org/A373345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, base
 * Formula:   a(n) = T(r,c) where T(r,k) = A373557(r,k) – 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 8151 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here r is the smallest integer such that the cumulative sum of the
 * popcounts of A371176(1),...,A371176(r) is ≥ n, and
 * c = n – (cumulative sum up to r–1).  The value T(r,c)
 * is the (c)-th element (counting from the largest) of the set encoded
 * by A371176(r).  The set is represented by the positions of the 1-bits
 * in the binary expansion of A371176(r) (positions are 1‑based with
 * 1 the least‑significant bit). */

/*  The implementation below builds the A371176 sequence lazily and
 *  stores the cumulative popcount totals to locate the proper row.
 *  All calculations use only unsigned arithmetic and bit‑wise
 *  operations; the final result is returned as seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: return the (pos)-th element (0‑based) of the set described
 * by the bit positions of k, listed in decreasing order of positions. */
static inline unsigned get_desc_pos(uint64_t k, unsigned pos)
{
    /* Find the highest set bit of k. */
    int maxbit = 63 - __builtin_clzll(k);
    unsigned idx = 0;
    for (int bit = maxbit; bit >= 0; --bit)
    {
        if (k & ((uint64_t)1 << bit))
        {
            if (idx == pos) return (unsigned)(bit + 1);  /* 1‑based position */
            ++idx;
        }
    }
    return 0;  /* unreachable for valid input */
}

/* Helper: test whether an integer k is a Schreier-encoded value.
 * A value is Schreier iff popcount(k) <= (position of its least
 * significant set bit).  The position is 1‑based. */
static inline bool is_schreier(uint64_t k)
{
    if (k == 0) return false;
    unsigned pop   = __builtin_popcountll(k);
    unsigned minpos = __builtin_ctzll(k) + 1;   /* position of lowest 1-bit */
    return pop <= minpos;
}

/* Cache for A371176 values and cumulative row lengths. */
struct A371_cache_s
{
    uint64_t *vals;      /* encoded values k */
    idx_t   *cumul;      /* cumulative counts of triangle entries */
    size_t   count;      /* number of rows cached so far */
    size_t   cap;        /* capacity of the allocated arrays */
    uint64_t next;       /* next candidate integer to test */
};

static struct A371_cache_s cache = {NULL, NULL, 0, 0, 1};

/* Ensure the cache contains at least 'need' entries in the triangle. */
static void ensure_cache(idx_t need)
{
    /* Generate new A371176 values until cumulative count reaches 'need'. */
    while (cache.count == 0 || cache.cumul[cache.count - 1] < need)
    {
        /* Find the next Schreier value. */
        uint64_t v;
        for (v = cache.next; ; ++v)
        {
            if (is_schreier(v))
                break;
        }
        cache.next = v + 1;

        /* Resize arrays if required. */
        if (cache.count >= cache.cap)
        {
            size_t new_cap = cache.cap ? cache.cap * 2 : 16;
            uint64_t *nv = (uint64_t*)realloc(cache.vals, new_cap * sizeof(uint64_t));
            idx_t    *nc = (idx_t*)realloc(cache.cumul, new_cap * sizeof(idx_t));
            if (!nv || !nc) abort();  /* allocation failure */
            cache.vals  = nv;
            cache.cumul = nc;
            cache.cap   = new_cap;
        }

        /* Store the new value and update cumulative count. */
        cache.vals[cache.count] = v;
        unsigned bc = __builtin_popcountll(v);
        if (cache.count == 0)
            cache.cumul[cache.count] = bc;
        else
            cache.cumul[cache.count] = cache.cumul[cache.count - 1] + bc;
        ++cache.count;
    }
}

/* Public function required by the assignment. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for offset below 1 */

    ensure_cache(n);

    /* Binary search for the row containing index n. */
    size_t l = 0, r = cache.count - 1;
    while (l < r)
    {
        size_t m = (l + r) >> 1;
        if (cache.cumul[m] < n)
            l = m + 1;
        else
            r = m;
    }
    size_t row = l;          /* 0‑based row index */

    idx_t prev_cum = (row == 0) ? 0 : cache.cumul[row - 1];
    unsigned pos_in_row = (unsigned)(n - prev_cum); /* 1‑based */

    uint64_t k = cache.vals[row];
    unsigned result = get_desc_pos(k, pos_in_row - 1); /* 0‑based */

    return (seq_t)result;
}
