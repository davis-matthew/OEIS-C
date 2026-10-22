/*
 * A330747  Number of values of k, 1 <= k <= n, with A049559(k) = A049559(n),
 *          where A049559(n) = gcd(n - 1, phi(n)).
 * https://oeis.org/A330747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 1≤k≤n | gcd(k-1,phi(k)) = gcd(n-1,phi(n)) }
 * Range:     u8 n <= 624, u16 n <= 9584, u32 n <= 9584, u64 n <= 9584,
 *            u128 n <= 9584, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Cached values of φ up to the largest n requested so far */
static idx_t *phi = NULL;
static idx_t max_phi_computed = 0;

/* Recompute φ[1..upto] using the standard sieve */
static void recompute_phi(idx_t upto)
{
    if (phi != NULL)
        free(phi);
    phi = (idx_t*)malloc((upto + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= upto; ++i)
        phi[i] = i;
    phi[1] = 1;
    for (idx_t i = 2; i <= upto; ++i) {
        if (phi[i] == i) {          /* i is prime */
            for (idx_t j = i; j <= upto; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    max_phi_computed = upto;
}

seq_t a(idx_t n)
{
    /* Ensure φ values are available up to n */
    if (n > max_phi_computed)
        recompute_phi(n);

    /* Compute f(n) = gcd(n-1, φ(n)) */
    seq_t fn = gcd((seq_t)n - 1, (seq_t)phi[n]);

    /* Count how many k ≤ n have the same value */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t fk = gcd((seq_t)k - 1, (seq_t)phi[k]);
        if (fk == fn)
            ++count;
    }
    return count;
}
