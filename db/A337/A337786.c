/*
 * A337786  Numbers of the form x^3 + x^2*y + x*y^2 + y^3 where x and y are
 *          positive integers, but having no such representation where x and y
 *          are coprime.
 * https://oeis.org/A337786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Homogeneous representation :  a is the sequence of all values
   of the form x^3 + x^2*y + x*y^2 + y^3 with x,y>0
   that possess at least one representation with gcd(x,y)>1
   but have no representation with gcd(x,y)=1.
   The code generates these values by inspecting all pairs
   (x,y) up to a growing bound, sorts the results, removes
   duplicates, and keeps only the required values.  */

/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility: gcd for seq_t */
#include "../utils/gcd.h"

/* Types supplied by the build are seq_t and idx_t.  */

/* Record for one computed value */
typedef struct {
    seq_t val;       /* the value x^3 + x^2*y + x*y^2 + y^3 */
    unsigned char cop;   /* 1 if the pair is coprime, 0 otherwise */
} Item;

/* Compare two Items by their value */
static int cmp_item(const void *a, const void *b)
{
    const Item *ia = (const Item *)a;
    const Item *ib = (const Item *)b;
    if (ia->val < ib->val) return -1;
    if (ia->val > ib->val) return 1;
    return 0;
}

/* Append a value to a dynamically growing seq_t array */
static void push_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* Generate at least target terms of the sequence.
   The generated array is stored in *seq_out and its length in *len_out. */
static void generate_sequence(size_t target, seq_t **seq_out, size_t *len_out)
{
    idx_t Xmax = 200;   /* initial bound for x and y */
    while (1) {
        size_t pairs = (size_t)Xmax * (size_t)Xmax;
        Item *items = (Item *)calloc(pairs, sizeof(Item));
        if (!items) abort();
        size_t items_len = 0;

        for (idx_t x = 1; x <= Xmax; ++x) {
            unsigned __int128 xs = (unsigned __int128)x * x;
            unsigned __int128 x3 = xs * x;
            for (idx_t y = 1; y <= Xmax; ++y) {
                unsigned __int128 ys = (unsigned __int128)y * y;
                unsigned __int128 y3 = ys * y;
                unsigned __int128 val128 = x3 + xs * y + (unsigned __int128)x * ys + y3;
                seq_t val = (seq_t)val128;
                unsigned char cop = (gcd((seq_t)x, (seq_t)y) == (seq_t)1) ? 1 : 0;
                items[items_len].val = val;
                items[items_len].cop = cop;
                ++items_len;
            }
        }

        /* Sort items by value to group equal values together */
        qsort(items, items_len, sizeof(Item), cmp_item);

        /* Collect final sequence values */
        seq_t *seq = NULL;
        size_t seq_cap = 0;
        size_t seq_len = 0;

        size_t i = 0;
        while (i < items_len) {
            seq_t cur_val = items[i].val;
            unsigned char has_coprime = 0;
            unsigned char has_not_coprime = 0;
            while (i < items_len && items[i].val == cur_val) {
                if (items[i].cop) has_coprime = 1;
                else has_not_coprime = 1;
                ++i;
            }
            if (has_not_coprime && !has_coprime) {
                push_seq(&seq, &seq_len, &seq_cap, cur_val);
                if (seq_len >= target) break;   /* no need to keep scanning */
            }
        }

        free(items);

        if (seq_len >= target) {
            *seq_out = seq;
            *len_out = seq_len;
            return;
        }

        /* Not enough terms yet – try with a larger bound */
        free(seq);
        Xmax *= 2;   /* increase search range */
    }
}

/* Public sequence function required by the build system */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static bool built = false;

    if (!built) {
        generate_sequence(12000, &cache, &cache_len);   /* safety margin */
        built = true;
    }

    /* No bounds checking – the caller guarantees n >= 1 */
    return cache[(size_t)(n - 1)];
}
