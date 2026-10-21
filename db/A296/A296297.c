/*
 * A296297  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n), where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296297: a(n) = a(n-1) + a(n-2) + n*b(n)
   a(0)=2, a(1)=4
   b(0)=1, b(1)=3, b(2)=5
   (a) and (b) are increasing complementary sequences. */

#include <stdlib.h>   /* malloc, realloc, bsearch */

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static idx_t   cap   = 0;
    static idx_t   a_cnt = 0;   /* number of a values stored */
    static seq_t  next_b = 0;   /* next candidate for b */

    /* --- Initialization --- */
    if (a_arr == NULL) {
        cap = 16;
        a_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        b_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (a_arr == NULL || b_arr == NULL) return (seq_t)0;

        /* initial a values */
        a_arr[0] = (seq_t)2;
        a_arr[1] = (seq_t)4;
        /* compute a(2) from the recurrence, using b(2)=5 */
        a_arr[2] = a_arr[0] + a_arr[1] + (seq_t)2 * (seq_t)5;
        a_cnt   = 3;          /* we now have a[0..2] */

        /* initial b values */
        b_arr[0] = (seq_t)1;
        b_arr[1] = (seq_t)3;
        b_arr[2] = (seq_t)5;
        /* next integer that has not yet been used in a or b */
        next_b   = (seq_t)6;
    }

    /* --- Fast return for already known terms --- */
    if (n < a_cnt) return a_arr[n];

    /* --- Ensure capacity --- */
    while (n >= cap) {
        idx_t new_cap = cap * 2;
        seq_t *na = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        seq_t *nb = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (na == NULL || nb == NULL) return (seq_t)0;
        a_arr = na;
        b_arr = nb;
        cap = new_cap;
    }

    /* --- Compute values from a_cnt up to n --- */
    for (idx_t i = a_cnt; i <= n; ++i) {
        /* Compute b(i) for i >= 3 (indices 0,1,2 are pre‑filled). */
        if (i >= 3) {
            while (1) {
                seq_t cand = next_b;
                /* cand must not appear in the already computed a's. */
                if (bsearch(&cand, a_arr, i, sizeof(seq_t), cmp_seq_t) == NULL) {
                    b_arr[i] = cand;
                    next_b   = cand + (seq_t)1;
                    break;
                }
                ++next_b;
            }
        }
        /* Compute a(i) using the recurrence. */
        seq_t ai = a_arr[i-1] + a_arr[i-2] + (seq_t)i * b_arr[i];
        a_arr[i] = ai;
        ++a_cnt;
    }

    return a_arr[n];
}
