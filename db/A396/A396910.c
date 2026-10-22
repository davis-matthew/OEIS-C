/*
 * A396910  Least k such that the set {J(n,1),J(n,2),...,J(n,k)} equals
 *          {1,2,...,n}, where J(n,k) is the Josephus survivor.
 * https://oeis.org/A396910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least k such that the set {J(n,1),J(n,2),...,J(n,k)}
   equals {1,2,…,n}.  J(n,k) is the survivor of the Josephus problem
   with n participants and step size k, computed by
   J = 0; for i = 2..n: J = (J + k) % i; survivor = J + 1.         */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safeguard for 0‑based calls */

    size_t sz = (size_t)n + 1;               /* indices 0..n, ignore 0       */
    bool *seen = (bool*)malloc(sz);          /* cast needed for C++          */
    if (!seen) return 0;                     /* allocation failure           */
    for (size_t i = 0; i < sz; ++i) seen[i] = false;
    size_t unique = 0;

    for (idx_t k = 1; ; ++k) {
        idx_t j = 0;
        for (idx_t i = 2; i <= n; ++i) {
            j = (j + k) % i;
        }
        idx_t survivor = j + 1;              /* 1‑based survivor             */
        if (!seen[survivor]) {
            seen[survivor] = true;
            ++unique;
            if (unique == n) {
                free(seen);
                return (seq_t)k;            /* cast idx_t to seq_t          */
            }
        }
    }
}
