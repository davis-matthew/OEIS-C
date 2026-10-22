/*
 * A399777  Positive integers n such that the central-cross sum divides the
 *          diagonal product in the (2*n + 1) X (2*n + 1) square spiral.
 * https://oeis.org/A399777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399777 – a(n) is the n‑th integer m for which the
   central‑cross sum of the (2*m+1)×(2*m+1) Ulam spiral divides
   the product of the entries on the two main diagonals.
   The algorithm incrementally tests m=1,2,…, storing each m
   that satisfies the divisibility until the requested n‑th term
   is found. */

#include <stdlib.h>

/* Return the n-th value of the sequence.  The first term is a(1) = 1. */
seq_t a(idx_t n)
{
    static seq_t *found = NULL;
    static size_t found_len = 0, found_cap = 0;
    static idx_t current_m = 0;   /* last m tested (0 at start) */
    static seq_t s_curr = 1;      /* s(0) = 1  – central‑cross sum */

    if (n == 0) return 0; /* undefined for index 0 */

    while (found_len < n) {
        /* advance to next m */
        ++current_m;
        seq_t km = (seq_t)current_m;

        /* update central‑cross sum: s(m) = s(m‑1) + 16*m² + 4 */
        seq_t add = km * km;               /* m²   */
        add = add * (seq_t)16;             /* 16*m² */
        add += (seq_t)4;                   /* + 4   */
        s_curr += add;                     /* s(m) */

        /* compute the product of diagonal entries modulo s(m) */
        seq_t prod_mod = 1;
        for (idx_t k = 1; k <= current_m; ++k) {
            seq_t kk  = (seq_t)k;
            seq_t kk2 = kk * kk;               /* k² */

            seq_t a1 = (seq_t)4 * kk2 - (seq_t)2 * kk + (seq_t)1;
            seq_t a2 = (seq_t)4 * kk2 + (seq_t)1;
            seq_t a3 = (seq_t)4 * kk2 + (seq_t)2 * kk + (seq_t)1;
            seq_t a4 = (seq_t)4 * kk2 + (seq_t)4 * kk + (seq_t)1;

            a1 %= s_curr;  a2 %= s_curr;  a3 %= s_curr;  a4 %= s_curr;

            seq_t temp = a1 * a2;
            temp %= s_curr;
            temp = temp * a3;
            temp %= s_curr;
            temp = temp * a4;
            temp %= s_curr;

            prod_mod = prod_mod * temp;
            prod_mod %= s_curr;

            if (prod_mod == 0) break;  /* product already divisible */
        }

        if (prod_mod == 0) {             /* m satisfies the condition */
            if (found_len == found_cap) {
                size_t new_cap = found_cap ? found_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(found,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();
                found = tmp;
                found_cap = new_cap;
            }
            found[found_len++] = km;
        }
    }

    return found[n - 1];
}
