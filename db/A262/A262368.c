/*
 * A262368  Expansion of f(x^2, x^2) * f(x, x^2)^2 in powers of x where f(, )
 *          is Ramanujan's general theta function.
 * https://oeis.org/A262368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A262368 – Expansion of f(x², x²) * f(x, x²)²
 *  Recurrence derived from the Euler transform:
 *     a(0) = 1
 *     a(n) = (1/n) * Σ_{i=0}^{n-1} a(i) * S[n-i]   (n ≥ 1)
 *  where   S(m) = Σ_{d|m} d * b_d
 *  and the period‑24 sequence b_d is
 *     [2, 2, -2, -3, 2, 0, 2, -1, -2, 2, 2, -5, 2, 2, -2, -1,
 *      2, 0, 2, -3, -2, 2, 2, -3].
 */

#include <stdlib.h>

/* static mutable state */
static seq_t *A = NULL;   /* coefficients a(n) */
static int   *S = NULL;   /* auxiliary sums S(m) */
static idx_t  len = 0;    /* size of allocated arrays (number of stored terms) */

/* extend precomputation to index n (inclusive) */
static void extend_to(idx_t n)
{
    if (n < len) return;                     /* already computed */

    idx_t new_len = n + 1;                   /* space for 0..n */
    seq_t *Anew    = (seq_t*)malloc(new_len * sizeof(seq_t));
    int   *Snew    = (int*)   malloc(new_len * sizeof(int));

    if (!Anew || !Snew) {                    /* allocation failure */
        if (Anew) free(Anew);
        if (Snew) free(Snew);
        exit(1);
    }

    /* copy already known coefficients */
    for (idx_t i = 0; i < len; ++i) Anew[i] = A[i];

    /* period‑24 sequence b_d */
    static const int B[24] = {
         2,  2, -2, -3,  2,  0,  2, -1, -2,  2,  2, -5,
         2,  2, -2, -1,  2,  0,  2, -3, -2,  2,  2, -3
    };

    /* initialise Snew; Snew[0] is unused */
    for (idx_t i = 1; i < new_len; ++i) Snew[i] = 0;

    /* compute S(m) for 1 ≤ m < new_len */
    for (idx_t k = 1; k < new_len; ++k) {
        int bval = B[(int)((k - 1) % 24)];   /* periodic value */
        if (bval == 0) continue;
        int add = (int)(k * bval);           /* k * b_k */
        for (idx_t m = k; m < new_len; m += k)
            Snew[m] += add;
    }

    /* fill missing coefficients using the recurrence */
    for (idx_t i = 0; i < new_len; ++i) {
        if (i == 0) {
            Anew[0] = (seq_t)1;              /* a(0) */
            continue;
        }
        long long sum = 0;                   /* intermediate sum fits in 64‑bit */
        for (idx_t j = 0; j < i; ++j) {
            long long aj = (long long)Anew[j];
            sum += aj * (long long)Snew[i - j];
        }
        Anew[i] = (seq_t)(sum / (long long)i);   /* division is exact */
    }

    /* free old storage */
    free(A);
    free(S);

    /* store new arrays */
    A = Anew;
    S = Snew;
    len = new_len;
}

/* public routine – returns a(n) (external linkage) */
seq_t a(idx_t n)
{
    extend_to(n);
    return A[n];
}
