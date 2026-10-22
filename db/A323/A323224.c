/*
 * A323224  A(n, k) = [x^k] C^n*x/(1 - x) where C = 2/(1 + sqrt(1 - 4*x)),
 *          square array read by ascending antidiagonals with n >= 0 and k >=
 *          0.
 * https://oeis.org/A323224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323224:
   a(n) is obtained by reading the triangle T(n,k) in anti‑diagonal order.
   T is defined recursively:
        T(0,0) = 0,
        T(0,k) = 1   for k ≥ 1
        T(n,0) = 0   for n ≥ 1
        T(n,n) = 1
        T(n,k) = T(n-1,k) + T(n,k-1)   for n ≥ 1, 1 ≤ k ≤ n-1
   (The recurrence also works for k > n using the same formula.)
   For a given index n we find the diagonal sum s = n + k such that
   s(s+1)/2 ≤ n < (s+1)(s+2)/2, set k = n − s(s+1)/2,
   and return T(s,k). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find diagonal s and position k on that diagonal */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    idx_t n_start = s * (s + 1) / 2;
    idx_t k = n - n_start;

    /* allocate a (s+1)×(s+1) table for T */
    idx_t dim = s + 1;
    size_t size = (size_t)dim * (size_t)dim;
    seq_t *T = (seq_t *)calloc(size, sizeof(seq_t));
    if (!T)
        abort(); /* memory failure */

    /* base row n = 0 */
    for (idx_t kk = 1; kk <= s; ++kk)
        T[kk] = 1;          /* T[0][kk] = 1 for kk ≥ 1 */

    /* fill the table using the recurrence */
    for (idx_t ni = 1; ni <= s; ++ni) {
        size_t base   = (size_t)ni * dim;
        size_t prevRow = (size_t)(ni - 1) * dim;
        for (idx_t kk = 1; kk <= s; ++kk) {
            size_t idx = base + kk;
            if (kk == ni)          /* T[ni][ni] = 1 */
                T[idx] = 1;
            else                    /* T[ni][kk] = T[ni-1][kk] + T[ni][kk-1] */
                T[idx] = T[prevRow + kk] + T[idx - 1];
        }
    }

    seq_t result = T[(size_t)s * dim + k];
    free(T);
    return result;
}
