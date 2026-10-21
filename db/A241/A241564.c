/*
 * A241564  Number of 3-element subsets of {1,...,n} whose sum has more than 3
 *          divisors.
 * https://oeis.org/A241564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 1888, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Algorithm:
   a(n) = total number of 3‑subsets {i<j<k} ⊆ {1..n}
          whose sum s = i+j+k has more than 3 positive divisors.
   The sums with 1 or 2 divisors cannot occur (the minimal sum is 6).
   Sums with exactly 3 divisors are prime squares, and sums that are
   prime have 2 divisors.  Therefore
      a(n) = C(n,3) – (number of triples with sum prime or prime‑square).
   The algorithm keeps, as it iterates over k = 1..n,
   a table `pairSum[t]` = number of pairs (i,j), 1≤i<j<k, with i+j = t.
   For each candidate sum T (prime or prime‑square) we add
      pairSum[T‑k]   (if T‑k>0)   to the “bad’’ count.
   After processing the current k we update `pairSum` with all new pairs
   involving k.
*/

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* total number of 3‑subsets */
    seq_t nn  = (seq_t)n;
    seq_t tot = nn * (nn - 1) * (nn - 2) / 6;

    /* ----  sieve primes up to 3*n  ---- */
    idx_t limit = 3 * n;

    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) return tot;

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;
        }
    }

    /* ----  collect primes and prime squares <= limit  ---- */
    size_t cap = limit / 10 + 10;
    idx_t *targets = (idx_t *)malloc(cap * sizeof(idx_t));
    size_t tcnt = 0;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;

        if (tcnt + 1 >= cap) {
            cap <<= 1;
            targets = (idx_t *)realloc(targets, cap * sizeof(idx_t));
        }
        targets[tcnt++] = p;               /* prime */

        /* prime square, avoiding overflow */
        if (p <= limit / p) {
            idx_t psq = p * p;             /* fits in idx_t */
            if (psq <= limit) {
                if (tcnt + 1 >= cap) {
                    cap <<= 1;
                    targets = (idx_t *)realloc(targets, cap * sizeof(idx_t));
                }
                targets[tcnt++] = psq;
            }
        }
    }

    /* ----  array counting pair sums (i<j<current k)  ---- */
    idx_t max_pair_sum = 2 * n;
    idx_t *pairSum = (idx_t *)calloc((size_t)(max_pair_sum + 1), sizeof(idx_t));
    if (!pairSum) {
        free(is_prime);
        free(targets);
        return tot;
    }

    seq_t bad = 0;   /* triples with sum prime or prime‑square */

    for (idx_t k = 1; k <= n; ++k) {
        for (size_t ti = 0; ti < tcnt; ++ti) {
            idx_t T = targets[ti];
            idx_t diff = T - k;
            if (diff > 0 && diff <= max_pair_sum)
                bad += (seq_t)pairSum[diff];
        }
        for (idx_t i = 1; i < k; ++i)
            ++pairSum[i + k];
    }

    free(is_prime);
    free(targets);
    free(pairSum);

    return tot - bad;
}
