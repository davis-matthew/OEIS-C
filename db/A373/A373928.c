/*
 * A373928  Number of compositions of 7*n-2 into parts 1 and 7.
 * https://oeis.org/A373928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of compositions of 7*n-2 into parts 1 and 7.
 *
 * Define the auxiliary sequence f(m) (m ≥ 0):
 *   f(0)   = 1,
 *   f(1) … f(6) = 1,
 *   f(m) = f(m-1) + f(m-7)          for m ≥ 7.
 *
 * Then a(n) = f(7*n-2).  This yields the linear recurrence
 *   a(n)=8*a(n‑1)-21*a(n‑2)+35*a(n‑3)-35*a(n‑4)+21*a(n‑5)-7*a(n‑6)+a(n‑7)
 * with initial values taken from the first terms of the sequence.
 */

#include <stdlib.h>

static seq_t *F = NULL;
static size_t F_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for the offset, but safe */

    /* Index in the auxiliary sequence f(m) */
    idx_t target = 7 * n - 2;
    size_t t = (size_t)target;

    /* Initialise the table if needed */
    if (F == NULL) {
        F_len = 1;
        F = (seq_t *)malloc(sizeof(seq_t) * F_len);
        if (!F) return 0;
        F[0] = (seq_t)1;                /* f(0) = 1 */
    }

    /* Extend the table to include index t */
    if (t >= F_len) {
        size_t new_len = t + 1;
        seq_t *tmp = (seq_t *)realloc(F, sizeof(seq_t) * new_len);
        if (!tmp) return 0;
        F = tmp;

        /* Compute the new entries using the recurrence */
        for (size_t i = F_len; i <= t; ++i) {
            if (i < 7) {
                F[i] = (seq_t)1;        /* f(1)…f(6) = 1 */
            } else {
                F[i] = F[i-1] + F[i-7];
            }
        }
        F_len = new_len;
    }

    return F[t];
}
