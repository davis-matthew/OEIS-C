/*
 * A294545  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - 1, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-2) + b(n-1) - 1,
 * where a(0) = 1, a(1) = 2, b(0) = 3,
 * and (a(n)) and (b(n)) are increasing complementary sequences.
 * The code builds the sequences iteratively, keeping the smallest
 * unused integer as the next element of b.  It caches results so
 * that calls with arbitrary (possibly decreasing) indices are
 * handled efficiently.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper binary search ---------- */
static bool seq_contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;          /* hi is exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return true;
    }
    return false;
}

static bool is_used(seq_t val,
                    const seq_t *a_arr, size_t a_len,
                    const seq_t *b_arr, size_t b_len)
{
    return seq_contains(a_arr, a_len, val) ||
           seq_contains(b_arr, b_len, val);
}

/* ---------- Cache and generation logic ---------- */
static seq_t* a_cache = NULL;
static seq_t* b_cache = NULL;
static size_t a_len   = 0;   /* number of stored a terms   */
static size_t b_len   = 0;   /* number of stored b terms   */
static size_t a_cap   = 0;   /* allocated capacity for a   */
static size_t b_cap   = 0;   /* allocated capacity for b   */
static seq_t  candidate = 1; /* smallest unused integer    */
static bool   initialized = false;

static void grow_array(seq_t **arr, size_t *cap, size_t new_len)
{
    if (*cap >= new_len)
        return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* insufficient memory */
    *arr = tmp;
    *cap = new_cap;
}

static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    /* initial allocation */
    a_cap = 16; b_cap = 16;
    a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
    b_cache = (seq_t*)malloc(b_cap * sizeof(seq_t));
    if (!a_cache || !b_cache) abort();

    /* set fixed initial terms */
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)2;
    a_len = 2;

    b_cache[0] = (seq_t)3;
    b_len   = 1;

    /* find first unused integer for b[1] */
    candidate = (seq_t)1;
    while (is_used(candidate, a_cache, a_len, b_cache, b_len))
        ++candidate;
    grow_array(&b_cache, &b_cap, b_len + 1);
    b_cache[b_len++] = candidate;
    ++candidate;   /* next candidate to be searched */
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    init_cache();
    if (n < a_len)
        return a_cache[n];

    /* Generate terms up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* compute a[i] */
        seq_t valA = a_cache[i-1] + a_cache[i-2] + b_cache[i-1] - (seq_t)1;
        grow_array(&a_cache, &a_cap, i + 1);
        a_cache[i] = valA;
        ++a_len;

        /* compute b[i] */
        grow_array(&b_cache, &b_cap, b_len + 1);
        while (is_used(candidate,
                       a_cache, a_len,
                       b_cache, b_len))
            ++candidate;
        b_cache[b_len++] = candidate;
        ++candidate;   /* prepare for next search */
    }
    return a_cache[n];
}
