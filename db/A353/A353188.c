/*
 * A353188  Number of partitions of n that contain at least one composite
 *          part.
 * https://oeis.org/A353188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) – A034891(n) a(n) = number of partitions of n
 *            that contain at least one composite part. For each n ≥ 0 this
 *            function returns that value
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: compute partitions of all numbers up to n using the
   pentagonal‑number recurrence (O(n √n)).  The result is stored
   in the array `p` (size n+1).  Caller is responsible for freeing the array.
*/
static void compute_partitions(idx_t n, seq_t *p)
{
    p[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t t = p[m - g1];
            if (k & 1) pos += t; else neg += t;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                t = p[m - g2];
                if (k & 1) pos += t; else neg += t;
            }
        }
        p[m] = pos - neg;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Allocate arrays for partitions p and q */
    size_t sz = (size_t)(n + 1);
    seq_t *p = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!p) return 0;
    seq_t *q = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!q) { free(p); return 0; }

    /* Compute p(n) using recurrence */
    compute_partitions(n, p);

    /* Initialize q: partitions using only 1 and primes */
    q[0] = 1;
    /* part = 1 */
    for (idx_t j = 1; j <= n; ++j) q[j] += q[j - 1];

    /* Sieve of Eratosthenes to find primes up to n */
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) { free(p); free(q); return 0; }
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) is_prime[j] = false;
        }
    }

    /* Process prime parts */
    for (idx_t part = 2; part <= n; ++part) {
        if (is_prime[part]) {
            for (idx_t j = part; j <= n; ++j) q[j] += q[j - part];
        }
    }

    /* Result: partitions containing at least one composite part */
    seq_t result = p[n] - q[n];

    /* Free temporary arrays */
    free(is_prime);
    free(q);
    free(p);
    return result;
}
