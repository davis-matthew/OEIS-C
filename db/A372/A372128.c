/*
 * A372128  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, a(2*n+1) = a(2*n) OR a(n) (where OR
 *          denotes the bitwise OR operator).
 * https://oeis.org/A372128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2n+1) = a(2n) OR a(n) (bitwise OR), a(1) = 1.
 *            Lexicographically earliest permutation of the positive integers
 *            satisfying the recurrence. The sequence is built incrementally:
 *            for each n ≥ 1, * a(2n) is the smallest unused integer c such
 *            that (c | a(n)) is unused and ≠ c; * a(2n+1) = a(2n) | a(n)
 * Range:     u8 n <= 149, u16 n <= 8413, u32 n <= 8413, u64 n <= 8413,
 *            u128 n <= 8413, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard headers. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static storage for the sequence values, allocated as needed. */
static seq_t *seq_arr  = NULL;   /* indexed from 1 */
static size_t seq_cap  = 0;      /* capacity (inclusive index of last allocated cell) */
static size_t seq_len  = 0;      /* highest index that has been computed */

/* Bitmap for fast "used" lookup.  indices are treated as unsigned long
   values (idx_t).  Elements with value ≤ bitmap_cap-1 are indexed
   directly. The bitmap is resized as values grow. */
static uint8_t *used_bitmap  = NULL;
static size_t  bitmap_cap    = 0;

/* Ensure seq_arr can hold indices up to 'need' (inclusive). */
static void grow_seq_arr(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap > 0 ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, (new_cap + 1) * sizeof(seq_t));
    if (!seq_arr) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* Ensure the bitmap can cover values up to 'v'.  Newly allocated bits
   are zero-initialised. */
static void ensure_bitmap(size_t v)
{
    if (v < bitmap_cap) return;
    size_t new_cap = bitmap_cap > 0 ? bitmap_cap : 256;
    while (new_cap <= v) new_cap <<= 1;
    uint8_t *new_map = (uint8_t *)realloc(used_bitmap, new_cap * sizeof(uint8_t));
    if (!new_map) exit(EXIT_FAILURE);
    /* Zero the newly added area. */
    memset(new_map + bitmap_cap, 0, (new_cap - bitmap_cap) * sizeof(uint8_t));
    used_bitmap = new_map;
    bitmap_cap  = new_cap;
}

/* Test whether a value has already appeared. */
static bool used_contains(seq_t val)
{
    /* Assume val fits into idx_t. */
    idx_t idx = (idx_t)val;
    ensure_bitmap((size_t)idx);
    return used_bitmap[(size_t)idx] != 0;
}

/* Record that a value has just been used. */
static void used_add(seq_t val)
{
    idx_t idx = (idx_t)val;
    ensure_bitmap((size_t)idx);
    used_bitmap[(size_t)idx] = 1;
}

/* Main function computing A372128. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Initialise data structures on first call. */
    if (!seq_arr)
    {
        grow_seq_arr(2);          /* we will at least need index 1 */
        seq_len = 0;
    }

    /* If the value has already been computed, return it. */
    if ((size_t)n <= seq_len) return seq_arr[(size_t)n];

    /* Build the sequence up to index n. */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i)
    {
        grow_seq_arr(i);          /* ensure storage for a[i] */

        if (i == 1)
        {
            seq_arr[1] = (seq_t)1;
            used_add((seq_t)1);
        }
        else if ((i & 1) == 0)    /* even index: a(2k) */
        {
            seq_t cand = (seq_t)1;
            while (true)
            {
                if (!used_contains(cand))
                {
                    seq_t orval = cand | seq_arr[i / 2];
                    if (cand != orval && !used_contains(orval))
                    {
                        seq_arr[i] = cand;
                        used_add(cand);
                        /* the corresponding a(2k+1) will be set when i+1 is processed */
                        break;
                    }
                }
                cand++;
            }
        }
        else                       /* odd index: a(2k+1) = a(2k) | a(k) */
        {
            size_t parent = (i - 1) / 2;
            seq_t orval = seq_arr[i - 1] | seq_arr[parent];
            seq_arr[i] = orval;
            used_add(orval);
        }
    }

    seq_len = (size_t)n;
    return seq_arr[(size_t)n];
}
