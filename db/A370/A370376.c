/*
 * A370376  Number of compositions of n where there are A025174(k) sorts of
 *          part k.
 * https://oeis.org/A370376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (1/3) * sum_{k=1..n} binom(3*k,k) * a(n-k) (n ≥
 *            1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom_3k_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_3k_k.h"

/* Return the n-th term of OEIS sequence A370376. */
seq_t a(idx_t n)
{
    /* ---------- caches of previously computed values ---------- */
    static seq_t *A = NULL;          /* a[0..A_cap-1]            */
    static seq_t *C = NULL;          /* C[k] = binom(3*k,k) for k>=0 */
    static idx_t A_cap   = 0;         /* total allocated slots for A  */
    static idx_t C_cap   = 0;         /* total allocated slots for C  */
    static idx_t A_used  = 0;         /* number of A terms constructed */
    static idx_t C_used  = 0;         /* up to which k C is valid     */

    /* ---------- grow cache for A up to n ---------- */
    if (n + (idx_t)1 > A_cap) {
        size_t new_cap = (A_cap == 0 ? 16 : A_cap * 2);
        while (new_cap <= n + (idx_t)1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        A = tmp;
        A_cap = (idx_t)new_cap;
    }

    /* ---------- grow cache for C up to n ---------- */
    if (n + (idx_t)1 > C_cap) {
        size_t new_cap = (C_cap == 0 ? 16 : C_cap * 2);
        while (new_cap <= n + (idx_t)1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
        C = tmp;
        C_cap = (idx_t)new_cap;
    }

    /* ---------- initialise a(0) ---------- */
    if (A_used == 0) {
        A[0] = (seq_t)1;
        A_used = 1;
    }

    /* ---------- compute missing C values ---------- */
    for (idx_t k = C_used; k <= n; ++k)
        C[k] = binom_3k_k(k);
    C_used = n + (idx_t)1;

    /* ---------- compute a[i] for i = A_used .. n ---------- */
    for (idx_t i = A_used; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = (idx_t)1; k <= i; ++k)
            sum += C[k] * A[i - k];
        A[i] = sum / (seq_t)3;          /* division is exact */
    }
    A_used = n + (idx_t)1;

    return A[n];
}
