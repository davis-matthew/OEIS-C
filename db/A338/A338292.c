/*
 * A338292  Lexicographically earliest sequence of positive integers such that
 *          for any distinct m and n, (a(m+1-a(m)), ..., a(m)) <>
 *          (a(n+1-a(n)), ..., a(n)).
 * https://oeis.org/A338292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lexicographically earliest sequence defined by the greedy rule:
 *  a(n) is the smallest positive integer k such that the block
 *  (a[n-k+1], … , a[n]) of length k has not appeared earlier.
 *  The sequence starts a(1)=1, a(2)=2, a(3)=2, a(4)=3, … as in OEIS A338292. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    idx_t start;      /* 1‑based start index of the tuple */
    idx_t len;        /* length of the tuple */
} tuple_t;

static seq_t   *a_arr = NULL;   /* sequence values, 1‑based indexing (a_arr[0] unused) */
static size_t  a_cap = 0;       /* current capacity of a_arr */
static size_t  a_len = 0;       /* number of values stored in a_arr */

static tuple_t *tuples = NULL;  /* descriptors of all previously seen tuples */
static size_t  t_cap = 0;       /* capacity of tuples array */
static size_t  t_len = 0;       /* number of tuples stored */

/* Allocate/reallocate a_arr to at least `needed` elements (including index 0). */
static void ensure_a_cap(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Allocate/reallocate tuples array to at least `needed` elements. */
static void ensure_t_cap(size_t needed)
{
    if (needed <= t_cap) return;
    size_t new_cap = t_cap ? t_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    tuples = (tuple_t *)realloc(tuples, new_cap * sizeof(tuple_t));
    t_cap = new_cap;
}

/* Check whether the length‑`len` block ending at `pos` has already appeared. */
static bool tuple_exists(idx_t pos, idx_t len)
{
    idx_t start_new = pos - len + 1;
    for (size_t i = 0; i < t_len; ++i) {
        if (tuples[i].len != len) continue;
        idx_t start_prev = tuples[i].start;
        bool equal = true;
        for (idx_t j = 0; j < len; ++j) {
            if (a_arr[start_new + j] != a_arr[start_prev + j]) {
                equal = false;
                break;
            }
        }
        if (equal) return true;
    }
    return false;
}

/* Compute the sequence up to at least `target` (target >= 1). */
static void compute_upto(idx_t target)
{
    if (target <= a_len) return;          /* already computed */

    /* Prepare storage. */
    ensure_a_cap(target + 1);    /* +1 for 1‑based convenience */
    ensure_t_cap(target + 1);

    /* Build sequence incrementally. */
    for (idx_t pos = a_len + 1; pos <= target; ++pos) {
        for (idx_t cand = 1; cand <= pos; ++cand) {
            a_arr[pos] = (seq_t)cand;          /* provisional value for comparison */
            if (!tuple_exists(pos, cand)) {
                /* Commit the chosen value and record the new tuple. */
                tuples[t_len].start = pos - cand + 1;
                tuples[t_len].len   = cand;
                ++t_len;
                break;
            }
        }
    }
    a_len = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, but harmless */
    compute_upto(n);
    return a_arr[n];
}
