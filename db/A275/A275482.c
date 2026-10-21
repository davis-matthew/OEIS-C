/*
 * A275482  The sum of the last digit of a(n) and the first digit of a(n+1) is
 *          a divisor of a(n) and a(n+1) concatenated.
 * https://oeis.org/A275482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   greedy construction. a(1) = 1; for n > 1, a(n) is the smallest
 *            integer not yet present such that the divisor (last digit of
 *            a(n‑1) + first digit of a(n)) divides the concatenation of
 *            a(n‑1) and a(n)
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow10.h"      /* seq_t pow10(idx_t) */
#include "../utils/digit_len.h" /* int digit_len(seq_t) */

/* -------------------- bitmap helpers -------------------- */
static uint64_t *used_bitmap   = NULL;
static size_t   bitmap_words   = 0;   /* number of 64‑bit words used */
static size_t   bitmap_cap     = 0;   /* number of bits represented */

/* Grow the bitmap to accommodate at least `new_cap` bits. */
static void grow_bitmap(size_t new_cap)
{
    if (new_cap <= bitmap_cap) return;
    size_t w = (new_cap + 63) >> 6;                /* words required */
    uint64_t *p = (uint64_t *)realloc(used_bitmap, w * sizeof(uint64_t));
    if (!p) abort();
    used_bitmap = p;
    if (w > bitmap_words) {
        for (size_t i = bitmap_words; i < w; ++i) used_bitmap[i] = 0;
    }
    bitmap_words = w;
    bitmap_cap   = w * 64ULL;
}

/* Test whether a small value `v < bitmap_cap` has been used. */
static bool used_small_contains(idx_t v)
{
    if (v >= bitmap_cap) return false;
    size_t w = v >> 6;
    uint64_t b = 1ULL << (v & 63);
    return (used_bitmap[w] & b) != 0ULL;
}

/* -------------------- large vector helpers -------------------- */
static seq_t *used_large   = NULL;
static size_t large_len    = 0;
static size_t large_cap    = 0;

/* Grow the vector used for large values. */
static void large_grow(void)
{
    if (large_cap == 0) {
        large_cap = 8;
        used_large = (seq_t *)calloc(large_cap, sizeof(seq_t));
    } else {
        large_cap <<= 1;
        used_large = (seq_t *)realloc(used_large, large_cap * sizeof(seq_t));
    }
    if (!used_large) abort();
}

/* Binary search: does the sorted vector contain `v`? */
static bool used_large_contains(seq_t v)
{
    size_t lo = 0, hi = large_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_large[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return (lo < large_len && used_large[lo] == v);
}

/* Insert `v` into the sorted vector. */
static void used_large_insert(seq_t v)
{
    size_t lo = 0, hi = large_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_large[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    if (large_len + 1 > large_cap) large_grow();
    if (lo < large_len)
        memmove(&used_large[lo + 1], &used_large[lo],
                (large_len - lo) * sizeof(seq_t));
    used_large[lo] = v;
    ++large_len;
}

/* Mark value `v` as used (in bitmap or vector). */
static void used_mark(seq_t v)
{
    if ((seq_t)bitmap_cap > v) {
        idx_t i = (idx_t)v;
        size_t w = i >> 6;
        uint64_t b = 1ULL << (i & 63);
        used_bitmap[w] |= b;
    } else {
        used_large_insert(v);
    }
}

/* -------------------- sequence helpers -------------------- */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap * 2 : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *p = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    if (!p) abort();
    seq = p;
    seq_cap = cap;
}

/* First decimal digit of a non‑negative value. */
static int first_digit(seq_t v)
{
    if (v == 0) return 0;
    int d = digit_len(v);
    seq_t p = pow10(d - 1);
    return (int)(v / p);
}

/* Last decimal digit of a non‑negative value. */
static int last_digit(seq_t v)
{
    return (int)(v % 10);
}

/* Concatenate decimal representations of `a` and `b`. */
static seq_t concat(seq_t a, seq_t b)
{
    int db = digit_len(b);
    seq_t p = pow10(db);
    return a * p + b;
}

/* -------------------- main function -------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; safety guard */

    /* (1) Initialise structures once. */
    if (!bitmap_cap) grow_bitmap(1ULL << 20);   /* start with 1 M bits */

    if (seq_len == 0) {                 /* seed the sequence */
        grow_seq(1);
        seq[0] = (seq_t)1;
        used_mark((seq_t)1);
        seq_len = 1;
    }

    /* (2) Generate terms up to position `n`. */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        idx_t cand_int = 1;             /* candidates start at 1 */

        while (1) {
            /* Grow bitmap if we are about to refer to an index beyond its range. */
            if (cand_int >= bitmap_cap)
                grow_bitmap(cand_int + 1);

            seq_t cand = (seq_t)cand_int;

            /* Skip numbers already used. */
            if (used_small_contains(cand_int) || used_large_contains(cand))
            {
                ++cand_int;
                continue;
            }

            int sum = last_digit(prev) + first_digit(cand);
            if (sum == 0) {              /* avoid division by zero */
                ++cand_int;
                continue;
            }

            seq_t con = concat(prev, cand);
            if (con % sum == 0) {        /* candidate satisfies the condition */
                grow_seq(seq_len + 1);
                seq[seq_len++] = cand;
                used_mark(cand);
                break;
            }
            ++cand_int;
        }
    }

    /* (3) Return the requested term. */
    return seq[n - 1];
}
