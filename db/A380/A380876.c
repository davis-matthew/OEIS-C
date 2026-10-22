/*
 * A380876  a(1) = 1; a(2) = 4; for n > 2, a(n) = least unused positive y such
 *          that gcd(y,n-1) > 1 and |y-n| > 1.
 * https://oeis.org/A380876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least unused positive y such that gcd(y,n-1)>1 and
 *            |y-n|>1. a(1)=1, a(2)=4. The function builds the sequence
 *            incrementally and memoises previously computed values. It
 *            handles any n>=1 and can be called in arbitrary order. All
 *            arithmetic is performed in seq_t
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *seq      = NULL;   /* array of computed a(i) in order */
static size_t seq_len  = 0;      /* number of elements stored in seq    */
static size_t seq_cap  = 0;      /* current capacity of seq array       */

static seq_t *used     = NULL;   /* sorted array of all values already used   */
static size_t used_len = 0;
static size_t used_cap = 0;

/* Utility: binary search in used[]; returns position if found,
   otherwise returns the index where the value should be inserted.  */
static size_t used_find(const seq_t *v, seq_t key)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used[mid] < key) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* Insert key into used[] maintaining sorted order.  */
static void used_insert(seq_t key)
{
    if (used_len == used_cap) {
        size_t newcap = used_cap ? used_cap << 1 : 16;
        used = (seq_t *)realloc(used, newcap * sizeof(seq_t));
        used_cap = newcap;
    }
    size_t pos = used_find(used, key);
    /* shift to make room */
    for (size_t i = used_len; i > pos; --i) used[i] = used[i-1];
    used[pos] = key;
    used_len++;
}

/* Ensure seq has capacity for at least newcap elements.  */
static void seq_ensure(size_t newcap)
{
    if (seq_len >= newcap) return;
    if (newcap == 0) newcap = 16;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* The function requested by the problem statement.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return sentinel */

    /* Build the sequence up to index n if necessary.  */
    while (seq_len < (size_t)n) {
        seq_ensure(seq_len + 1);   /* grow seq storage as needed */

        if (seq_len == 0) {
            seq[0] = (seq_t)1;
            used_insert(seq[0]);
        } else if (seq_len == 1) {
            seq[1] = (seq_t)4;
            used_insert(seq[1]);
        } else {
            /* compute a(k) where k = seq_len + 1 (since seq is 0‑based) */
            idx_t k = (idx_t)seq_len + 1;
            seq_t limit = (seq_t)(k - 1);          /* gcd argument */
            seq_t cand = (seq_t)1;
            seq_t found = 0;
            while (1) {
                /* |cand - k| > 1 condition */
                if (cand > (seq_t)k) {
                    if (cand - (seq_t)k <= (seq_t)1) goto next_cand;
                } else {
                    if ((seq_t)k - cand <= (seq_t)1) goto next_cand;
                }

                /* unused check via binary search */
                if (used_find(used, cand) < used_len && used[used_find(used, cand)] == cand)
                    goto next_cand;

                /* gcd check */
                if (gcd(cand, limit) > (seq_t)1) {
                    found = cand;
                    break;
                }
next_cand:
                cand++;   /* try next integer */
            }
            seq[seq_len] = found;
            used_insert(found);
        }
        seq_len++;
    }

    return seq[n-1];   /* zero‑based array, but n is 1‑based */
}
