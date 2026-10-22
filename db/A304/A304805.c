/*
 * A304805  Solution (a(n)) of the complementary equation a(n) = b(n) + b(5n)
 *          ; see Comments.
 * https://oeis.org/A304805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 9361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304805 :  a(n) = b(n) + b(5n)
 *  where  b(0) = 1,
 *  b(k)   = least positive integer not yet used (in either a or b),
 *  a(k)   = b(k) + b(5k).
 *
 *  The function below memoises the two sequences so that any call
 *  for n >= 0 yields the correct a(n) in O(1) amortised time.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ---------- dynamic integer array helpers ---------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t c = *cap ? *cap << 1 : 64;
    while (c < need) c <<= 1;
    *arr = (seq_t *)realloc(*arr, c * sizeof(seq_t));
    if (!*arr) abort();
    *cap = c;
}

/* ---------- used‑number bitset ---------- */
static uint8_t  *used      = NULL;
static size_t   used_size = 0; /* number of entries in 'used' */

static void grow_used(idx_t v)
{
    if ((size_t)v < used_size) return;
    size_t new_size = used_size ? used_size << 1 : 256;
    while (new_size <= (size_t)v) new_size <<= 1;
    used = (uint8_t *)realloc(used, new_size);
    if (!used) abort();
    memset(used + used_size, 0, new_size - used_size);
    used_size = new_size;
}

static void mark_used(idx_t v)
{
    grow_used(v);
    used[(size_t)v] = 1;
}

static idx_t next_unused = 1;   /* candidate for the next least unused */

static idx_t find_least_unused(void)
{
    for (;;) {
        grow_used(next_unused);
        if (!used[(size_t)next_unused]) {
            return next_unused++;
        }
        ++next_unused;
    }
}

/* ---------- storage for the two sequences ---------- */
static seq_t *b_arr = NULL; /* b(k)  values   */
static seq_t *a_arr = NULL; /* a(k)  values   */
static size_t b_cap = 0, a_cap = 0;
static idx_t  next_b_index = 0;   /* next b index to compute */

/* compute all values up to index n, including all required b(5n)   */
static void ensure_upto(idx_t n)
{
    idx_t target = n * 5;         /* highest b index that influences a(n) */
    if (target < next_b_index) return;

    for (idx_t i = next_b_index; i <= target; ++i) {
        idx_t cand = find_least_unused();
        seq_t val  = (seq_t)cand;

        /* store b(i) */
        grow_seq(&b_arr, &b_cap, i + 1);
        b_arr[i] = val;
        mark_used(cand);

        /* when i is a multiple of 5, compute the corresponding a   */
        if ((i % 5) == 0) {
            idx_t ai = i / 5;
            seq_t a_val = b_arr[ai] + b_arr[i];
            grow_seq(&a_arr, &a_cap, ai + 1);
            a_arr[ai] = a_val;
            mark_used((idx_t)a_val);
        }
    }
    next_b_index = target + 1;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    ensure_upto(n);
    return a_arr[n];
}
