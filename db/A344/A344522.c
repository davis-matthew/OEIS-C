/*
 * A344522  a(n) = Sum_{1 <= i, j, k <= n} gcd(i,j,k).
 * https://oeis.org/A344522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * floor(n/k)^3
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1465, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#   include <new>
#endif

/* Static cache for φ(1)…φ(max) */
static seq_t *phi_cache = NULL;   /* phi values, 1‑indexed */
static idx_t  phi_max   = 0;

/* Ensure phi_cache holds φ(k) for 1 ≤ k ≤ n */
static void ensure_phi(idx_t n)
{
    if (n <= phi_max)
        return;

    /* Allocate new array */
#if defined(__cplusplus)
    seq_t *new_phi = new seq_t[n+1];
#else
    seq_t *new_phi = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_phi) return; /* allocation failure: work‑arounds not required here */
#endif

    /* Initialize φ(k)=k */
    for (idx_t i = 1; i <= n; ++i)
        new_phi[i] = (seq_t)i;

    /* Compute φ-values using sieve */
    for (idx_t i = 2; i <= n; ++i) {
        if (new_phi[i] == (seq_t)i) {            /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                new_phi[j] = new_phi[j] / (seq_t)i * (seq_t)(i - 1);
            }
        }
    }

    /* Replace old cache, freeing it with the matching deallocator */
#if defined(__cplusplus)
    delete[] phi_cache;
#else
    free(phi_cache);
#endif
    phi_cache = new_phi;
    phi_max   = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset <=1; behaviour below 1 is unspecified */

    ensure_phi(n);

    seq_t total = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;                          /* floor(n/k) */
        seq_t t3 = (seq_t)t * (seq_t)t * (seq_t)t;/* t^3 */
        total += phi_cache[k] * t3;
    }
    return total;
}
