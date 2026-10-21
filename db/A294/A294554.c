/*
 * A294554  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + 2, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+b(n-1)+b(n-2)+2 with a(0)=1 , a(1)=2 ,
 *            b(0)=3 , b(1)=4 (b(k) is the smallest integer > b(k-1) not
 *            appearing in a[0..k-1])
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* use the utilities for memory management */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static size_t a_cap = 0;
static size_t a_len = 0;   /* number of a terms currently stored */

/* ensure arrays have capacity at least new_cap */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= a_cap)
        return;

    seq_t *na = alloc_seq_array(new_cap);
    seq_t *nb = alloc_seq_array(new_cap);
    if (!na || !nb)
        abort();   /* allocation failure */

    /* copy existing data */
    for (size_t i = 0; i < a_len; ++i) {
        na[i] = a_seq[i];
        nb[i] = b_seq[i];
    }

    /* free old storage */
    if (a_seq) free_seq_array(a_seq);
    if (b_seq) free_seq_array(b_seq);

    a_seq = na;
    b_seq = nb;
    a_cap = new_cap;
}

/* binary search for x in a_seq[0 .. a_len-1] */
static bool a_contains(seq_t x)
{
    size_t lo = 0, hi = a_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_seq[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < a_len && a_seq[lo] == x);
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;

    if (target >= a_len) {
        /* grow capacity as needed */
        ensure_capacity(target + 1);

        /* initialise first terms if not done */
        if (a_len == 0) {
            a_seq[0] = (seq_t)1;   /* a(0) */
            b_seq[0] = (seq_t)3;   /* b(0) */
            a_len = 1;
        }

        /* compute up to target */
        for (size_t k = a_len; k <= target; ++k) {
            if (k == 1) {
                a_seq[1] = (seq_t)2;   /* a(1) */

                /* b(1) is the least integer > b(0)=3 not in a */
                seq_t cand = b_seq[0] + (seq_t)1;
                while (a_contains(cand))
                    cand += (seq_t)1;
                b_seq[1] = cand;

                a_len = 2;
            } else {
                seq_t a1 = a_seq[k - 1];
                seq_t a2 = a_seq[k - 2];
                seq_t b1 = b_seq[k - 1];
                seq_t b2 = b_seq[k - 2];

                a_seq[k] = a1 + a2 + b1 + b2 + (seq_t)2;

                /* compute b(k) */
                seq_t cand = b_seq[k - 1] + (seq_t)1;
                while (a_contains(cand))
                    cand += (seq_t)1;
                b_seq[k] = cand;

                ++a_len;
            }
        }
    }

    return a_seq[target];
}
