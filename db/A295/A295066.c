/*
 * A295066  Solution of the complementary equation a(n) = 2*a(n-2) + b(n-1),
 *          where a(0) = 1, a(1) = 3, b(0) = 2, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295066
   a(0) = 1, a(1) = 3, b(0) = 2
   a(n) = 2*a(n-2) + b(n-1)   (n ≥ 2)
   (a(n)) and (b(n)) are the two increasing complementary sequences
   that together contain all positive integers. */

/* ---------- standard headers ---------- */
#include <stddef.h>   /* size_t, NULL */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, abort */

/* ---------- helper: binary search for first >= key ---------- */
static idx_t lower_bound_seq(const seq_t *arr, size_t len, seq_t key)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)len;          /* hi is the length of the array */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ---------- helper: ensure capacity of a dynamic array ---------- */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : *cap;
        while (new_cap < needed)
            new_cap <<= 1;               /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failed */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ---------- static storage for memoisation ---------- */
static seq_t *a_seq = NULL;          /* array of a values   */
static seq_t *b_seq = NULL;          /* array of b values   */
static size_t a_cap = 0, b_cap = 0;  /* current capacities  */
static idx_t   len      = 0;          /* number of terms computed (a[0..len-1]) */

/* ---------- public function ----------------------------------- */
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (a_seq == NULL) {
        a_cap = 4;  b_cap = 4;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!a_seq || !b_seq) abort();

        a_seq[0] = (seq_t)1;   /* a(0) = 1 */
        b_seq[0] = (seq_t)2;   /* b(0) = 2 */
        len      = 1;          /* have a[0] and b[0] */
    }

    /* compute up to the requested index if necessary */
    if (n >= len) {
        for (idx_t k = len; k <= n; ++k) {

            /* allocate space for new a[k] and b[k] */
            ensure_capacity(&a_seq, &a_cap, (size_t)(k + 1));
            ensure_capacity(&b_seq, &b_cap, (size_t)(k + 1));

            /* compute a(k) */
            if (k == 1) {
                a_seq[1] = (seq_t)3;               /* a(1) = 3 */
            } else {
                a_seq[k] = a_seq[k - 2] * (seq_t)2 + b_seq[k - 1];
            }

            /* compute b(k): smallest integer > b(k-1)
               that is not an a-term up to index k. */
            seq_t cand = b_seq[k - 1] + (seq_t)1;
            while (true) {
                idx_t pos = lower_bound_seq(a_seq, (size_t)(k + 1), cand);
                if (pos < (size_t)(k + 1) && a_seq[pos] == cand) {
                    ++cand;                 /* cand is an a-term; skip it */
                } else {
                    break;                  /* cand is free for b(k)      */
                }
            }
            b_seq[k] = cand;
        }
        len = n + 1;                         /* now we have a[0..n] & b[0..n] */
    }

    return a_seq[n];
}
