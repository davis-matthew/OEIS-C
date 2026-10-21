/*
 * A296283  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1)*b(n), where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2,
 *          b(2) = 5, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)*b(n) a(0) = 3, a(1) = 4
 *            b(0) = 1, b(1) = 2, b(2) = 5 a() and b() are complementary
 *            increasing sequences
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static local state for memoisation */
    static seq_t *a_seq = NULL;
    static seq_t *b_seq = NULL;
    static size_t a_len = 0;
    static size_t b_len = 0;
    static size_t a_cap = 0;
    static size_t b_cap = 0;
    static seq_t cur_b = (seq_t)6;      /* next candidate integer for b[] */
    static size_t a_ptr = 0;             /* first a[] >= cur_b */

    /* lazy initialisation */
    if (a_len == 0) {
        /* initial capacities */
        a_cap = 16;
        b_cap = 16;
        a_seq = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_seq = (seq_t*)malloc(b_cap * sizeof(seq_t));
        /* a[0], a[1] */
        a_seq[0] = (seq_t)3;
        a_seq[1] = (seq_t)4;
        a_len = 2;
        /* b[0], b[1], b[2] */
        b_seq[0] = (seq_t)1;
        b_seq[1] = (seq_t)2;
        b_seq[2] = (seq_t)5;
        b_len = 3;
        /* set a_ptr to first a >= cur_b */
        a_ptr = 0;
        while (a_ptr < a_len && a_seq[a_ptr] < cur_b) a_ptr++;
    }

    /* return immediately if already computed */
    if ((idx_t)n < a_len)
        return a_seq[n];

    /* ensure capacity for a and b arrays up to n */
    size_t need = (size_t)n + 1;
    if (need > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
    if (need > b_cap) {
        size_t new_cap = b_cap ? b_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        b_seq = (seq_t*)realloc(b_seq, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }

    /* generate terms up to n */
    for (idx_t i = (idx_t)a_len; i <= n; ++i) {
        seq_t bi;
        if (i < 3) {
            bi = b_seq[i];
        } else {
            /* find next missing integer (b[i]) */
            while (1) {
                /* skip a values lower than cur_b */
                while (a_ptr < a_len && a_seq[a_ptr] < cur_b) a_ptr++;
                if (a_ptr < a_len && a_seq[a_ptr] == cur_b) {
                    /* cur_b is an a value – skip it */
                    cur_b++;
                    a_ptr++;
                    continue;
                }
                bi = cur_b;
                cur_b++;
                break;
            }
            b_seq[i] = bi;
            b_len = (size_t)i + 1;
        }

        seq_t ai;
        if (i == 0) {
            ai = a_seq[0];
        } else if (i == 1) {
            ai = a_seq[1];
        } else {
            /* product = b[i-2]*b[i-1]*b[i] */
            seq_t prod = b_seq[i-2];
            prod = prod * b_seq[i-1];
            prod = prod * bi;
            ai = a_seq[i-1] + a_seq[i-2] + prod;
        }
        a_seq[i] = ai;
        a_len = (size_t)i + 1;
    }

    return a_seq[n];
}
