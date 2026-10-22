/*
 * A330521  Lexicographically earliest sequence of distinct positive terms
 *          such that the digitsum of a(n) ends a(n+1).
 * https://oeis.org/A330521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   digit_sum, digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A330521 – Lexicographically earliest sequence of distinct positive terms
 *  such that the decimal representation of a(n) ends with the digit‑sum of a(n‑1).
 *
 *  The implementation constructs the sequence incrementally.  For each possible
 *  digit‑sum value we keep a "suffix bucket" that stores the smallest unused
 *  integer ending with that digit‑sum.  A new term is obtained by taking the
 *  bucket corresponding to the previous term's digit‑sum, advancing it to the
 *  next unused value, inserting that value into the global used set (kept
 *  sorted), and updating the bucket's pointer.  The algorithm is O(n log m)
 *  where m is the current sequence length (≤ n), comfortably below the
 *  2‑second limit for n≈10 000.
 *
 *  All memory allocations use explicit casts so the code compiles both for
 *  C11 and C++17 (seq_t may be an arbitrary‑precision integer class).
 *
 *  Uses helper utilities:
 *      digit_sum(seq_t v)          – sum of decimal digits
 *      digits_of(idx_t x)          – number of decimal digits of x
 *      pow10(idx_t e)              – 10ᵉ
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_sum.h"
#include "../utils/digits_of.h"
#include "../utils/pow10.h"

/* ----------  Used‑set (sorted array)  ---------- */
static seq_t *used_arr = NULL;      /* sorted array of used numbers   */
static size_t used_cap = 0;         /* capacity of used_arr          */
static size_t used_count = 0;       /* number of elements used_arr   */

static void used_init(void)
{
    used_cap = 16;
    used_arr = (seq_t *)calloc(used_cap, sizeof(seq_t));
    used_count = 0;
}

static bool used_contains(seq_t key)
{
    size_t lo = 0, hi = used_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < used_count && used_arr[lo] == key);
}

static void used_insert(seq_t key)
{
    if (used_count == used_cap) {
        used_cap <<= 1;
        used_arr = (seq_t *)realloc(used_arr, used_cap * sizeof(seq_t));
    }
    /* binary search for insertion point */
    size_t lo = 0, hi = used_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* shift right to make room */
    for (size_t i = used_count; i > lo; --i)
        used_arr[i] = used_arr[i - 1];
    used_arr[lo] = key;
    used_count++;
}

/* ----------  Suffix helper  ---------- */
typedef struct {
    seq_t ds;   /* the digit‑sum value for this bucket   */
    seq_t cur;  /* smallest unused number ending with ds */
    seq_t base; /* 10^digits(ds), step between candidates */
} suffix_entry_t;

static suffix_entry_t *suffixes = NULL;
static size_t suffixes_len = 0;
static size_t suffixes_cap = 0;

/* linear search – there are at most a few dozen distinct digit‑sums */
static size_t suffix_find_or_create(seq_t ds)
{
    for (size_t i = 0; i < suffixes_len; ++i) {
        if (suffixes[i].ds == ds)
            return i;
    }

    if (suffixes_len == suffixes_cap) {
        size_t new_cap = suffixes_cap ? suffixes_cap * 2 : 8;
        suffixes = (suffix_entry_t *)realloc(suffixes,
                                             new_cap * sizeof(suffix_entry_t));
        suffixes_cap = new_cap;
    }

    suffix_entry_t *e = &suffixes[suffixes_len++];
    e->ds = ds;
    e->base = pow10((idx_t)digits_of((idx_t)ds));
    e->cur = ds; /* will be advanced to the next unused value on use */
    return suffixes_len - 1;
}

/* ----------  Sequence storage  ---------- */
static seq_t *seq = NULL;      /* array holding the sequence   */
static size_t seq_cap   = 0;    /* capacity of seq array        */
static size_t seq_len   = 0;    /* number of terms generated    */

static void seq_init(void)
{
    seq_cap = 64;
    seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_len = 1;
    seq[0] = 1;

    used_init();
    used_insert(1);

    suffix_find_or_create(1); /* bucket for digit‑sum 1 is needed immediately */
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for 0, matching the requirement */

    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        seq_init();
    }

    /* Grow the sequence until we have n terms */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t ds   = digit_sum(prev);

        size_t idx = suffix_find_or_create(ds);
        suffix_entry_t *e = &suffixes[idx];

        /* find next unused candidate ending with ds */
        seq_t cand = e->cur;
        while (used_contains(cand))
            cand += e->base;
        seq_t val = cand;

        /* store the term */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = val;
        used_insert(val);

        /* advance the bucket pointer */
        cand = val + e->base;
        while (used_contains(cand))
            cand += e->base;
        e->cur = cand;
    }

    return seq[n - 1];
}
