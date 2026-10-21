/*
 * A297830  Solution of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          and (b(n)) is the increasing sequence of positive integers not in
 *          (a(n)). See Comments.
 * https://oeis.org/A297830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(1)*b(n‑1) – a(0)*b(n‑2) + 2*n a(0)=1, a(1)=2, b(0)=3,
 *            b(1)=4 b(n) is the increasing sequence of positive integers not
 *            in a(n)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_CAP 20005          /* enough for n up to ~10000 */

/* static storage for the sequences */
static seq_t a_array[MAX_CAP];
static idx_t a_len = 0;

static seq_t b_array[MAX_CAP];
static idx_t b_len = 0;

static seq_t next_candidate = (seq_t)5;   /* next integer to consider for b */

/* binary search to test if 'value' is in a_array[0 .. a_len-1] */
static int a_is_in_a(seq_t value)
{
    idx_t lo = 0, hi = a_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (a_array[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < a_len && a_array[lo] == value);
}

/* lazy initialization of the sequences */
static void init_seq(void)
{
    if (a_len != 0) return;   /* already initialized */

    a_array[0] = (seq_t)1;
    a_array[1] = (seq_t)2;
    a_len = 2;

    b_array[0] = (seq_t)3;
    b_array[1] = (seq_t)4;
    b_len = 2;

    next_candidate = (seq_t)5;
}

/* public function as required */
seq_t a(idx_t n)
{
    init_seq();

    /* compute terms up to n if necessary */
    for (idx_t idx = a_len; idx <= n; ++idx) {
        seq_t val;
        if (idx == 0) {
            val = (seq_t)1;
        } else if (idx == 1) {
            val = (seq_t)2;
        } else {
            /* use the recurrence */
            seq_t b1 = b_array[idx - 1];
            seq_t b2 = b_array[idx - 2];
            val = a_array[1] * b1 - a_array[0] * b2
                  + (seq_t)(2 * idx);
        }

        a_array[a_len] = val;
        ++a_len;

        /* generate the corresponding b[idx] after a[idx] is known */
        if (idx >= 2) {
            while (a_is_in_a(next_candidate))
                ++next_candidate;
            b_array[b_len] = next_candidate;
            ++b_len;
            ++next_candidate;
        }
    }

    return a_array[n];
}
