/*
 * A294562  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - b(n-2) + 1, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) - b(n-2) + 1 with a(0)=1,
 *            a(1)=2, b(0)=3, b(1)=4 The sequences a() and b() are increasing
 *            complementary sequences
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static void ensure_capacity(seq_t **arr, size_t *cap, size_t cur_len, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
#if defined(__cplusplus)
    seq_t *new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < cur_len; ++i) new_arr[i] = (*arr)[i];
    delete[] *arr;
    *arr = new_arr;
    *cap = new_cap;
#else
    seq_t *new_arr;
    if (*arr == NULL)
        new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    else
        new_arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = new_arr;
    *cap = new_cap;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *a_seq = NULL, *b_seq = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static size_t a_len = 0, b_len = 0;
    static size_t a_idx = 0;           /* scanning index for a() values   */
    static seq_t next_candidate = 0;   /* smallest unused positive integer */

    /* Initialise on first call. */
    if (a_seq == NULL) {
        ensure_capacity(&a_seq, &a_cap, 0, 1);
        ensure_capacity(&b_seq, &b_cap, 0, 1);
        a_seq[0] = (seq_t)1;
        b_seq[0] = (seq_t)3;
        a_len = b_len = 1;
        next_candidate = b_seq[0] + (seq_t)1;   /* 4 */
        a_idx = 0;
    }

    size_t target = (size_t)n;
    while (a_len <= target) {
        if (a_len == 1) {                      /* compute a(1) and b(1) */
            ensure_capacity(&a_seq, &a_cap, a_len, a_len + 1);
            a_seq[a_len] = (seq_t)2;
            ++a_len;

            seq_t cand = next_candidate;
            while (a_idx < a_len && a_seq[a_idx] <= cand) {
                if (a_seq[a_idx] == cand) cand++;
                ++a_idx;
            }
            ensure_capacity(&b_seq, &b_cap, b_len, b_len + 1);
            b_seq[b_len] = cand;
            ++b_len;
            next_candidate = cand + (seq_t)1;
        } else {                                /* general case (n >= 2) */
            size_t i = a_len;
            seq_t ai = a_seq[i-1] + a_seq[i-2]
                     + (b_seq[i-1] - b_seq[i-2]) + (seq_t)1;

            ensure_capacity(&a_seq, &a_cap, a_len, a_len + 1);
            a_seq[i] = ai;
            ++a_len;

            seq_t cand = next_candidate;
            while (a_idx < a_len && a_seq[a_idx] <= cand) {
                if (a_seq[a_idx] == cand) cand++;
                ++a_idx;
            }
            ensure_capacity(&b_seq, &b_cap, b_len, b_len + 1);
            b_seq[b_len] = cand;
            ++b_len;
            next_candidate = cand + (seq_t)1;
        }
    }
    return a_seq[target];
}
