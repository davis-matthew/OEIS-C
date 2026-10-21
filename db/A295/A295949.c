/*
 * A295949  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n) for n ≥ 2, with a(0)=1, a(1)=2,
 *            b(0)=3, b(1)=4, b(2)=5. The sequences a() and b() are increasing
 *            complementary sequences, i.e. together they contain each
 *            positive integer exactly once
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for computed terms */
static seq_t *a_arr = NULL;
static size_t a_len = 0, a_cap = 0;      /* a[0..a_len-1] computed */
static seq_t *b_arr = NULL;
static size_t b_len = 0, b_cap = 0;      /* b[0..b_len-1] computed */

/* ensure capacity for a_arr */
static inline void a_ensure_cap(size_t needed)
{
    if (a_cap < needed) {
        size_t new_cap = a_cap ?: 8;
        while (new_cap < needed) new_cap <<= 1;
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
}

/* ensure capacity for b_arr */
static inline void b_ensure_cap(size_t needed)
{
    if (b_cap < needed) {
        size_t new_cap = b_cap ?: 8;
        while (new_cap < needed) new_cap <<= 1;
        b_arr = (seq_t*)realloc(b_arr, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }
}

/* binary search: does array contain val? */
static inline bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo < len && arr[lo] == val;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialization on first call */
    if (!a_arr) {
        a_ensure_cap(8);
        b_ensure_cap(8);
        /* base values */
        a_arr[0] = 1;
        a_arr[1] = 2;
        a_len = 2;
        b_arr[0] = 3;
        b_arr[1] = 4;
        b_arr[2] = 5;
        b_len = 3;
        /* compute a(2) */
        seq_t a2 = a_arr[1] + a_arr[0] + b_arr[2];
        a_ensure_cap(a_len + 1);
        a_arr[a_len++] = a2;
    }

    /* compute terms up to n if not yet present */
    while (a_len <= (size_t)n) {
        size_t i = a_len;           /* next index to compute */

        /* find b(i): smallest integer > b(i-1) not used yet */
        seq_t candidate = b_arr[b_len - 1] + 1;
        while (contains(a_arr, a_len, candidate) ||
               contains(b_arr, b_len, candidate)) {
            candidate = candidate + 1;
        }
        b_ensure_cap(b_len + 1);
        b_arr[b_len++] = candidate;

        /* compute a(i) = a(i-1) + a(i-2) + b(i) */
        seq_t ai = a_arr[a_len - 1] + a_arr[a_len - 2] + candidate;
        a_ensure_cap(a_len + 1);
        a_arr[a_len++] = ai;
    }

    return a_arr[n];
}
