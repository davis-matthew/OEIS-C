/*
 * A296846  Solution of the complementary equation a(n) = a(n-1) + a(n-2) -
 *          b(n-2), where a(0) = 3, a(1) = 5, b(0) = 1, b(1) = 2, b(2) = 4,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296846
   a(n) = a(n-1) + a(n-2) - b(n-2),
   a(0)=3, a(1)=5,
   b(0)=1, b(1)=2, b(2)=4,
   a() and b() are increasing complementary sequences. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* helper allocation wrappers making C and C++ compatible */
static void *xmalloc(size_t sz) { return malloc(sz); }
static void *xrealloc(void *p, size_t sz) { return realloc(p, sz); }

/* static memoisation storage */
static seq_t *a_arr = NULL;          /* a(n) values                      */
static seq_t *b_arr = NULL;          /* b(k) values                      */
static idx_t   a_cap = 0, a_len = 0; /* capacity & current length of a_arr */
static idx_t   b_cap = 0, b_len = 0; /* capacity & current length of b_arr */
static seq_t   b_last = 0;           /* last produced value of b         */

/* ensure a_arr can hold at least 'need' elements */
static void ensure_a_cap(idx_t need)
{
    if (need <= a_cap) return;
    idx_t new_cap = a_cap ? a_cap << 1 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    a_arr = (seq_t *)xrealloc(a_arr, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* ensure b_arr can hold at least 'need' elements */
static void ensure_b_cap(idx_t need)
{
    if (need <= b_cap) return;
    idx_t new_cap = b_cap ? b_cap << 1 : 16;
    while (new_cap < (size_t)need) new_cap <<= 1;
    b_arr = (seq_t *)xrealloc(b_arr, new_cap * sizeof(seq_t));
    b_cap = new_cap;
}

/* binary search in a_arr[0 .. upto-1] for value v */
static bool a_contains(seq_t v, idx_t upto)
{
    idx_t lo = 0, hi = upto;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (a_arr[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return (lo < upto && a_arr[lo] == v);
}

/* generate b elements from current b_len up to target (inclusive) */
static void compute_b_up_to(idx_t target)
{
    while (b_len <= target) {
        ensure_b_cap(b_len + 1);          /* grow array if needed */
        seq_t cand = b_last + 1;
        while (a_contains(cand, a_len)) ++cand;
        b_arr[b_len] = cand;
        b_last = cand;
        ++b_len;
    }
}

/* initialise sequence tables with the given starting values */
static void init_sequences(void)
{
    if (a_len == 0) {
        ensure_a_cap(2);
        a_arr[0] = 3;
        a_arr[1] = 5;
        a_len = 2;
    }
    if (b_len == 0) {
        ensure_b_cap(3);
        b_arr[0] = 1;
        b_arr[1] = 2;
        b_arr[2] = 4;
        b_len = 3;
        b_last = b_arr[2];
    }
}

/* public function required by the platform */
seq_t a(idx_t n)
{
    init_sequences();

    /* extend a_arr up to index n, computing b as required */
    while (a_len <= n) {
        idx_t i = a_len;                /* index to compute */

        /* a(i) = a(i-1) + a(i-2) - b(i-2) */
        seq_t val = a_arr[i - 1] + a_arr[i - 2] - b_arr[i - 2];

        ensure_a_cap(i + 1);
        a_arr[i] = val;
        a_len = i + 1;

        /* after adding a(i), we need b(i-1) for the next step */
        if (b_len <= i - 1) compute_b_up_to(i - 1);
    }

    return a_arr[n];
}
