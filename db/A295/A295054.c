/*
 * A295054  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(1) + b(2) + ... + b(n-1), where a(0) = 1, a(1) = 2, b(0) = 3,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + Σ_{i=1}^{n-1} b(i); a(0) = 1, a(1) = 2,
 *            b(0) = 3, (a(n)) and (b(n)) are increasing complementary
 *            sequences
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_cap(void **p, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    *p = realloc(*p, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static local state */
    static seq_t *a_vals = NULL;   static size_t a_cap = 0;   static idx_t a_len = 0;
    static seq_t *b_vals = NULL;   static size_t b_cap = 0;   static idx_t b_len = 0;
    static seq_t sum_b = 0;        /* sum of b(1) .. b(k) */

    static seq_t *used = NULL;     static size_t used_cap = 0; static size_t used_cnt = 0;
    static size_t used_idx = 0;    /* index into used[] for searching */
    static seq_t next_candidate = 1; /* next integer to test as b value */

    /* first call initialisation */
    if (a_len == 0)
    {
        /* a(0) and a(1) */
        ensure_cap((void**)&a_vals, &a_cap, 2);
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)2;
        a_len = 2;

        /* b(0) */
        ensure_cap((void**)&b_vals, &b_cap, 1);
        b_vals[0] = (seq_t)3;
        b_len = 1;
        sum_b = 0;       /* b(0) is not part of the sum */

        /* used numbers: 1, 2, 3 */
        ensure_cap((void**)&used, &used_cap, 3);
        used[0] = (seq_t)1;
        used[1] = (seq_t)2;
        used[2] = (seq_t)3;
        used_cnt = 3;

        used_idx = 0;
        next_candidate = 1;
    }

    /* compute up to requested n */
    while (a_len <= n)
    {
        /* need b[a_len-1] before computing a[a_len] */
        if (b_len <= a_len - 1)
        {
            /* find the smallest unused integer */
            seq_t cand = next_candidate;
            while (1)
            {
                /* skip used numbers that are < cand */
                while (used_idx < used_cnt && used[used_idx] < cand)
                    ++used_idx;

                if (used_idx < used_cnt && used[used_idx] == cand)
                {
                    ++cand;          /* cand is already used */
                    continue;
                }

                /* cand is unused – this is the next b value */
                next_candidate = cand + 1;

                /* append cand to used list */
                ensure_cap((void**)&used, &used_cap, used_cnt + 1);
                used[used_cnt++] = cand;

                /* append cand to b sequence */
                ensure_cap((void**)&b_vals, &b_cap, b_len + 1);
                b_vals[b_len++] = cand;

                /* accumulate sum_b (exclude b(0)) */
                if (b_len > 1)   /* b(1) or later */
                    sum_b += cand;
                break;
            }
        }

        /* compute next a value */
        seq_t a_val = a_vals[a_len-1] + a_vals[a_len-2] + sum_b;
        ensure_cap((void**)&a_vals, &a_cap, a_len + 1);
        a_vals[a_len++] = a_val;

        /* mark a_val as used */
        ensure_cap((void**)&used, &used_cap, used_cnt + 1);
        used[used_cnt++] = a_val;
    }

    return a_vals[n];
}
