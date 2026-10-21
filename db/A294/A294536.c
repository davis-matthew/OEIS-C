/*
 * A294536  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) - 1, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A294536:
 *    a(0) = 1, a(1) = 2, b(0) = 3
 *    For n ≥ 2:
 *       a(n) = a(n−1) + a(n−2) + b(n−2) – 1
 *       b(n) = least positive integer not equal to any a(0)…a(n)
 *              and not equal to any b(0)…b(n−1)
 *    The sequences a() and b() are complementary and strictly increasing.
 *    This implementation constructs the sequences on demand and caches
 *    all already‑computed values.  Binary search is used to test whether
 *    a candidate for b(n) is already equal to an a value; since a() is
 *    increasing, this test is O(log n).  The algorithm runs in O(n log n)
 *    time with a tiny constant factor and satisfies all other requirements.
 */

#include <stdlib.h>
#include <stddef.h>

static idx_t   computed_max = (idx_t)(-1);   /* highest n for which a[] is defined */
static seq_t * a_arr = NULL;                 /* array of a values        */
static seq_t * b_arr = NULL;                 /* array of b values        */
static size_t  arr_cap = 0;                  /* current allocation size  */

/* Ensure the arrays can hold indices up to n (inclusive). */
static void grow_to(idx_t n)
{
    if ((size_t)(n + 1) <= arr_cap) return;
    size_t new_cap = arr_cap ? arr_cap * 2 : 8;
    while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    arr_cap = new_cap;
}

/* Binary search: return 1 if *x* is in a_arr[0..high] (inclusive). */
static int a_contains(idx_t high, seq_t x)
{
    idx_t lo = 0;
    idx_t hi = high;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (a_arr[mid] == x) return 1;
        if (a_arr[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* First call: initialise the starting terms. */
    if (computed_max == (idx_t)(-1)) {
        grow_to(1);
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)2;
        b_arr[0] = (seq_t)3;

        /* compute b(1) : smallest unused after 1,2,3 */
        seq_t cand = b_arr[0] + (seq_t)1;
        while (a_contains(1, cand)) ++cand;
        b_arr[1] = cand;

        computed_max = 1;
    }

    /* If the requested term is already known, return it. */
    if (n <= computed_max) return a_arr[n];

    /* Extend the cache up to n. */
    grow_to(n);

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t ai = a_arr[i-1] + a_arr[i-2] + b_arr[i-2] - (seq_t)1;
        a_arr[i] = ai;

        /* Compute b(i): least positive integer not in a[0..i] or earlier b. */
        seq_t cand = b_arr[i-1] + (seq_t)1;          /* start after previous b */
        while (a_contains(i, cand)) ++cand;        /* skip a-values */
        b_arr[i] = cand;
    }
    computed_max = n;
    return a_arr[n];
}
