/*
 * A296223  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0) - 1, where a(0) = 1, a(1) = 3,
 *          b(0) = 2, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{n-1} a(i)*b(n-1-i) - 1; initial values: a(0)=1,
 *            a(1)=3, b(0)=2. a() and b() are increasing complementary
 *            sequences
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static size_t a_len = 0, b_len = 0;
    static bool *used_small = NULL;
    static size_t used_small_size = 0;
    static idx_t next_b_candidate = 1;
    static bool init = false;

    /* one‑time initialization */
    if (!init) {
        /* allocate initial capacities */
        a_cap = 32;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_len = 0;

        b_cap = 32;
        b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b_len = 0;

        used_small_size = 64;
        used_small = (bool *)calloc(used_small_size, sizeof(bool));

        /* initial known terms */
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)3;
        a_len = 2;
        if (1 < used_small_size) used_small[1] = true;
        if (3 < used_small_size) used_small[3] = true;

        b_arr[0] = (seq_t)2;
        b_len = 1;
        if (2 < used_small_size) used_small[2] = true;

        init = true;
    }

    /* extend storage if needed */
    if (a_cap < 16) a_cap = 16;

    while (a_len <= n) {
        idx_t cur = a_len; /* compute a[cur] */

        /* ensure we have enough b terms: need indices 0 .. cur-1 */
        while (b_len < cur) {
            /* find next unused integer */
            idx_t cand = next_b_candidate;
            while (true) {
                if ((size_t)cand >= used_small_size) {
                    size_t new_sz = cand + 64;
                    bool *tmp = (bool *)realloc(used_small,
                                                new_sz * sizeof(bool));
                    /* zero new area */
                    memset(tmp + used_small_size,
                           0,
                           (new_sz - used_small_size) * sizeof(bool));
                    used_small = tmp;
                    used_small_size = new_sz;
                }
                if (!used_small[(size_t)cand]) break;
                cand++;
            }
            next_b_candidate = cand + 1;

            /* append to b array */
            if (b_cap <= b_len) {
                b_cap = b_cap ? b_cap * 2 : 4;
                b_arr = (seq_t *)realloc(b_arr, b_cap * sizeof(seq_t));
            }
            b_arr[b_len] = (seq_t)cand;
            if (cand < used_small_size) used_small[(size_t)cand] = true;
            b_len++;
        }

        /* compute convolution sum */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < cur; ++i) {
            seq_t prod = a_arr[i] * b_arr[cur - 1 - i];
            sum += prod;
        }
        a_arr[cur] = sum - (seq_t)1;

        /* mark new a term as used if it fits in our small table */
        if (a_arr[cur] <= (seq_t)(used_small_size - 1))
            used_small[(size_t)a_arr[cur]] = true;

        /* ensure a_array capacity */
        if (a_cap <= a_len) {
            a_cap = a_cap ? a_cap * 2 : 4;
            a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        }
        a_len++;
    }

    return a_arr[n];
}
