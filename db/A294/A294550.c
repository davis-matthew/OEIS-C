/*
 * A294550  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   complimentary sequences a and b satisfy * a(0)=1, a(1)=2, b(0)=3
 *            * a(n)=a(n-1)+a(n-2)+b(n-1)+b(n-2) (n≥2) * The b sequence is the
 *            least unused positive integer after * all previous a and b
 *            values, i.e. a and b are complementary * increasing sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* binary search: does arr[0..size-1] contain target? */
static bool contains(const seq_t *arr, size_t size, seq_t target)
{
    size_t lo = 0, hi = size;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < size && arr[lo] == target);
}

/* ensure the array can hold at least "need" elements */
static void ensure_cap(idx_t need, seq_t **arr, size_t *cap)
{
    if (*cap < (size_t)need) {
        size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
        if (new_cap < (size_t)need) new_cap = (size_t)need;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static storage for a and b */
    static seq_t *a_seq = NULL;
    static seq_t *b_seq = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static size_t a_len = 0, b_len = 0;   /* number of elements currently filled */

    /* Ensure capacity up to n+1 for a and b */
    ensure_cap(n + 1, &a_seq, &a_cap);
    ensure_cap(n + 1, &b_seq, &b_cap);

    /* initialise base cases if not done */
    if (a_len < 1) {
        a_seq[0] = (seq_t)1;
        a_len = 1;
    }
    if (a_len < 2) {
        a_seq[1] = (seq_t)2;
        a_len = 2;
    }
    if (b_len < 1) {
        b_seq[0] = (seq_t)3;
        b_len = 1;
    }
    if (b_len < 2) {
        /* compute b(1) = smallest unused integer after 1,2,3 */
        seq_t cand = b_seq[0] + (seq_t)1;               /* 4 */
        while (contains(a_seq, 2, cand))
            cand = cand + (seq_t)1;                    /* next candidate */
        b_seq[1] = cand;
        b_len = 2;
    }

    /* extend the sequences up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* compute a(i) using recurrence */
        a_seq[i] = a_seq[i - 1] + a_seq[i - 2]
                 + b_seq[i - 1] + b_seq[i - 2];

        /* compute b(i) : smallest unused integer > b(i-1) */
        seq_t cand = b_seq[i - 1] + (seq_t)1;
        while (contains(a_seq, i + 1, cand))
            cand = cand + (seq_t)1;                    /* skip a‑values */
        b_seq[i] = cand;
    }

    /* update lengths for next calls */
    a_len = (size_t)n + 1;
    b_len = (size_t)n + 1;

    return a_seq[n];
}
