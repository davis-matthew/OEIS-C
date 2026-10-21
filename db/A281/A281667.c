/*
 * A281667  Expansion of Sum_{i>=1} mu(i)^2*x^i/(1 + x^i) * Product_{j>=1} (1
 *          + mu(j)^2*x^j), where mu() is the Moebius function (A008683).
 * https://oeis.org/A281667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281667: Total number of parts in all partitions of n into
   distinct squarefree parts.
   The function uses a dynamic programming recurrence:
     P[0] = 1, A[0] = 0
     For each squarefree k ≤ n:
         P[n] += P[n-k]
         A[n] += A[n-k] + P[n-k]
   Memoised tables P and A are extended on demand, so the function
   works for any order of calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for unsigned 64‑bit integers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    for (int k = 63; k >= 0; --k) {
        idx_t bit = 1ULL << k;
        unsigned __int128 cand = (unsigned __int128)(r + bit) * (r + bit);
        if (cand <= n)
            r += bit;
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n=0, return 0 */
        return 0;

    static seq_t *P = NULL;          /* number of partitions */
    static seq_t *A = NULL;          /* total parts in those partitions */
    static idx_t cur_max = 0;        /* largest n that has been processed */

    if (n > cur_max) {
        idx_t new_max = n;

        /* Allocate new tables (malloc must be cast for C++). */
        seq_t *newP = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        seq_t *newA = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
        if (!newP || !newA) {        /* allocation failure */
            free(newP);
            free(newA);
            return 0;
        }

        /* Initialise base case. */
        newP[0] = 1;
        newA[0] = 0;
        for (idx_t i = 1; i <= new_max; ++i) {
            newP[i] = 0;
            newA[i] = 0;
        }

        /* Compute all square‑free integers up to new_max. */
        idx_t limit = isqrt_u64(new_max);
        uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        if (is_prime) {
            for (idx_t i = 2; i <= limit; ++i)
                is_prime[i] = 1;
            for (idx_t i = 2; i * i <= limit; ++i) {
                if (is_prime[i]) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_prime[j] = 0;
                }
            }
        }

        for (idx_t k = 1; k <= new_max; ++k) {
            bool sqfree = true;
            idx_t sq_limit = isqrt_u64(k);
            for (idx_t p = 2; p <= sq_limit; ++p) {
                if (is_prime && !is_prime[p])
                    continue;
                if (k % p == 0) {
                    idx_t sq = p * p;
                    if (k % sq == 0) {
                        sqfree = false;
                        break;
                    }
                }
            }
            if (sqfree) {
                for (idx_t m = new_max; m >= k; --m) {
                    newP[m] = newP[m] + newP[m - k];
                    newA[m] = newA[m] + newA[m - k] + newP[m - k];
                }
            }
        }

        free(is_prime);

        free(P);
        free(A);
        P = newP;
        A = newA;
        cur_max = new_max;
    }

    return A[n];
}
