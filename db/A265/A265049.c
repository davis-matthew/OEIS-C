/*
 * A265049  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 53, u64 n <= 113, u128 n <= 233,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A265049
   Recurrence (derived from the generating function):
     a(0) = 1, a(1) = 4, a(2) = 9, a(3) = 17, a(4) = 29, a(5) = 47,
     a(6) = 74, a(7) = 113, a(8) = 170, a(9) = 253, a(10) = 374, a(11) = 550
   For n ≥ 12:
       a(n) = 3 a(n‑1) – 4 a(n‑2) + 4 a(n‑3) – 3 a(n‑4) + 2 a(n‑5)
              – 2 a(n‑6) + 3 a(n‑7) – 4 a(n‑8) + 4 a(n‑9) – 3 a(n‑10) + a(n‑11)
*/

#include <stdlib.h>
#include <stdint.h>

static const seq_t BASE[12] = {
    1, 4, 9, 17, 29, 47, 74, 113, 170, 253, 374, 550
};

static const int d[12] = {0, 3, -4, 4, -3, 2, -2, 3, -4, 4, -3, 1};

seq_t a(idx_t n)
{
    if (n <= 11)
        return BASE[n];

    /* Allocate array for all needed terms */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)
        return 0;  /* allocation failure – unlikely */

    /* Initialize initial values */
    for (size_t i = 0; i <= 11; ++i)
        arr[i] = BASE[i];

    /* Compute terms from 12 up to n */
    for (size_t i = 12; i <= (size_t)n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 11; ++k) {
            seq_t term = arr[i - k];
            if (d[k] >= 0)
                pos += (seq_t)d[k] * term;
            else
                neg += (seq_t)(-d[k]) * term;
        }
        arr[i] = pos - neg;   /* pos ≥ neg, so unsigned subtraction is safe */
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}
