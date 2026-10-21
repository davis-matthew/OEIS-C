/*
 * A296555  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296555 – complementary sequence a(n)
 *
 *  Recurrence (for n ≥ 2):
 *      a(n) = a(n-1) + a(n-2) + b(n) + n
 *
 *  Initial values:
 *      a(0) = 1,  a(1) = 2
 *      b(0) = 3,  b(1) = 4,  b(2) = 5
 *
 *  The sequences (a(n)) and (b(n)) are increasing complementary integer
 *  sequences.  b(n) is the smallest unused positive integer after all a(i)
 *  (i < n) and b(i) (i < n) have been generated.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *a_arr   = NULL;   /* holds a(0)..a(computed_max)   */
    static size_t a_cap   = 0;
    static seq_t *b_arr   = NULL;   /* holds b(0)..b(computed_max+1) */
    static size_t b_cap   = 0;
    static idx_t computed_max = 1;   /* a(1) already known            */

    /* state used by the complement generator */
    static idx_t pos_a          = 0; /* next index in a_arr to inspect   */
    static idx_t pos_b          = 0; /* next index in b_arr to inspect   */
    static seq_t next_candidate = (seq_t)6;   /* smallest unused > all used */

    /* initialise the cache on first call ----------------------------------- */
    if (!a_arr) {
        /* a(0) = 1, a(1) = 2 */
        a_cap = 4;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)2;

        /* b(0) = 3, b(1) = 4, b(2) = 5 */
        b_cap = 4;
        b_arr = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b_arr[0] = (seq_t)3;
        b_arr[1] = (seq_t)4;
        b_arr[2] = (seq_t)5;      /* b(2) is known from the outset */

        pos_a = 0;
        pos_b = 0;
        next_candidate = (seq_t)6;
    }

    /* return immediately if already cached --------------------------------- */
    if (n <= computed_max) return a_arr[n];

    /* compute missing values up to n --------------------------------------- */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* enlarge a_arr if necessary */
        if ((size_t)i >= a_cap) {
            size_t new_cap = a_cap * 2;
            if (new_cap <= (size_t)i) new_cap = i + 1;
            seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
            if (!tmp) { free(a_arr); free(b_arr); return 0; }
            a_arr = tmp;
            a_cap = new_cap;
        }

        /* enlarge b_arr for b(i+1) (which is needed even on the last step)  */
        if (i + 1 >= (idx_t)b_cap) {
            size_t new_cap = b_cap * 2;
            if (new_cap <= (size_t)(i + 1)) new_cap = i + 2;
            seq_t *tmp = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
            if (!tmp) { free(a_arr); free(b_arr); return 0; }
            b_arr = tmp;
            b_cap = new_cap;
        }

        /* recurrence */
        seq_t a_prev2 = a_arr[i - 2];
        seq_t a_prev1 = a_arr[i - 1];
        seq_t b_curr  = b_arr[i];
        seq_t a_val   = a_prev2 + a_prev1 + b_curr + (seq_t)i;
        a_arr[i]      = a_val;

        /* determine b(i+1) – the smallest unused integer after all a ≤ i
           and b ≤ i have been seen                                            */
        seq_t candidate = next_candidate;
        for (;;) {
            /* advance pos_a past any a[] elements < candidate */
            while (pos_a <= i && a_arr[pos_a] < candidate) ++pos_a;
            /* advance pos_b past any b[] elements < candidate */
            while (pos_b <= i && b_arr[pos_b] < candidate) ++pos_b;
            /* is candidate already used? */
            int used =
                (pos_a <= i && a_arr[pos_a] == candidate) ||
                (pos_b <= i && b_arr[pos_b] == candidate);
            if (!used) break;
            ++candidate;   /* try next integer */
        }
        b_arr[i + 1] = candidate;          /* store b(i+1)   */
        next_candidate = candidate + 1;     /* for subsequent searches */
    }

    computed_max = n;
    return a_arr[n];
}
