/*
 * A384155  a(n) is the number of binary strings of length n whose shortest
 *          run of 1s is of length 3.
 * https://oeis.org/A384155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f_3(n) – f_4(n), where f_k(n) counts binary strings of
 *            length n whose all runs of 1s have length at least k. f_k
 *            satisfies the linear recurrences f_k(0)=1, f_k(1)=… , and for
 *            n≥1 f_k(n) = f_k(n-1) + f_k(n-k-1) + … + f_k(n-1) (implemented
 *            via state transitions below). The implementation use...
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *a_arr = NULL;   /* a[0..max_n] */
    static idx_t max_n = 0;       /* largest n computed */

    /* states for f_3 (states 0..3) and f_4 (states 0..4) */
    static seq_t f3_state[4] = {1, 0, 0, 0};  /* length 0 */
    static seq_t f4_state[5] = {1, 0, 0, 0, 0};

    /* initialise cache on first call */
    if (!a_arr) {
        a_arr = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!a_arr) return 0;   /* memory error */
        a_arr[0] = 0;           /* a(0) = 0   */
        max_n = 0;
    }

    /* expand cache up to required n */
    if (n > max_n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* memory error */
        a_arr = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* ----- update f_3 states ----- */
            seq_t f3_next0 = f3_state[0] + f3_state[3];
            seq_t f3_next1 = f3_state[0];
            seq_t f3_next2 = f3_state[1];
            seq_t f3_next3 = f3_state[2] + f3_state[3];
            f3_state[0] = f3_next0;
            f3_state[1] = f3_next1;
            f3_state[2] = f3_next2;
            f3_state[3] = f3_next3;
            seq_t f3_count = f3_state[0] + f3_state[3];

            /* ----- update f_4 states ----- */
            seq_t f4_next0 = f4_state[0] + f4_state[4];
            seq_t f4_next1 = f4_state[0];
            seq_t f4_next2 = f4_state[1];
            seq_t f4_next3 = f4_state[2];
            seq_t f4_next4 = f4_state[3] + f4_state[4];
            f4_state[0] = f4_next0;
            f4_state[1] = f4_next1;
            f4_state[2] = f4_next2;
            f4_state[3] = f4_next3;
            f4_state[4] = f4_next4;
            seq_t f4_count = f4_state[0] + f4_state[4];

            /* store a(i) */
            a_arr[i] = f3_count - f4_count;
        }
        max_n = n;
    }

    return a_arr[n];
}
