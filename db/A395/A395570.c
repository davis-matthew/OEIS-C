/*
 * A395570  Array read by ascending antidiagonals: A(n, k) = Sum_{j=0..n}
 *          Stirling2(n, j)*3^j*(k)_j.
 * https://oeis.org/A395570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395570 : array read by ascending antidiagonals
 *
 * a(n) = Σ_{j=0}^{min(n',k')} Stirling2(n',j) * 3^j * (k')_j
 * where n' and k' are the coordinates of n in the triangular array
 * read by antidiagonals, i.e. n = n'(n'+1)/2 + k'.
 *
 * The computation uses the recurrence for Stirling numbers of the
 * second kind and a simple loop over j.  It runs in O(n'^2) time,
 * where n' = O(√n), which is easily below 2 s for n ≤ 10 000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1. Convert linear index n to antidiagonal coordinates (n', k'). */
    idx_t s = 0;
    while (1) {
        unsigned __int128 next = (unsigned __int128)(s + 1) * (s + 2) / 2;
        if (next > n) break;
        ++s;
    }
    unsigned __int128 prev = (unsigned __int128)s * (s + 1) / 2;
    idx_t pos_in_diag = n - (idx_t)prev;          /* position along the diagonal */
    idx_t nprime = s - pos_in_diag;              /* row index in the tri‑array */
    idx_t kprime = pos_in_diag;                  /* column index in the tri‑array */

    /* 2. Compute Stirling numbers S(n', j) for j = 0..n' using a 1‑D array. */
    idx_t N = nprime;
    seq_t *st = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!st) return 0;  /* allocation failed – return 0 as a safe fallback */

    for (idx_t j = 0; j <= N; ++j) st[j] = 0;
    st[0] = 1; /* S(0,0) = 1 */

    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            /* S(i,j) = S(i-1,j-1) + j * S(i-1,j) */
            st[j] = st[j - 1] + (seq_t)j * st[j];
        }
        st[0] = 0; /* S(i,0) = 0 for i > 0 */
    }

    /* 3. Sum the terms of the definition. */
    idx_t maxj = (kprime < nprime) ? kprime : nprime;
    seq_t result = 0;
    seq_t pow3   = 1;
    seq_t fall    = 1; /* falling factorial (k')_0 = 1 */

    for (idx_t j = 0; j <= maxj; ++j) {
        if (j > 0) {
            pow3 *= 3;
            fall *= (kprime - j + 1);
        }
        seq_t term = st[j] * pow3 * fall;
        result += term;
    }

    free(st);
    return result;
}
