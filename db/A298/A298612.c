/*
 * A298612  The number of concave polygon classes.
 * https://oeis.org/A298612
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(d) * Σ_{k=1}^{⌊(n-3)/d⌋} C(n/d, k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The double sum counts necklaces with 1 ≤ #white ≤ n-3.)              */

/* Utility: ensure we have φ values up to n.                          */
#include <stdlib.h>
#include <stdint.h>

static idx_t *phi_cache = NULL;   /* φ values, index from 0..max_n */
static idx_t phi_cache_size = 0;  /* number of entries currently stored */

/* Sieve to compute φ for all i ≤ n.                                   */
static void ensure_phi(idx_t n) {
    if (phi_cache && phi_cache_size >= n + 1) return;   /* already big enough */

    /* enlarge the array */
    idx_t new_size = n + 1;
    idx_t *new_phi = (idx_t *)realloc(phi_cache, new_size * sizeof(idx_t));
    if (!new_phi) exit(1);
    phi_cache = new_phi;
    phi_cache_size = new_size;

    /* initially φ[i] = i, then adjust for primes */
    for (idx_t i = 0; i <= n; ++i) phi_cache[i] = i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_cache[i] == i) {                /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi_cache[j] -= phi_cache[j] / i;
            }
        }
    }
}

/* Main sequence function                                           */
seq_t a(idx_t n) {
    if (n < 3) return 0;          /* offset not defined below 3 */

    /* Build φ up to n */
    ensure_phi(n);

    /* Enumerate all divisors of n (≤ sqrt(n) + pairs) */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[divcnt++] = i;
            if (i != n / i) divs[divcnt++] = n / i;
        }
    }

    seq_t total = 0;

    /* For each divisor d | n */
    for (size_t di = 0; di < divcnt; ++di) {
        idx_t d = divs[di];
        idx_t m = n / d;
        idx_t K = (n - 3) / d;           /* maximum k with w = d*k ≤ n-3 */
        if (K == 0) continue;

        seq_t sum = 0;
        seq_t binom = 1;                 /* C(m,0) */
        for (idx_t k = 1; k <= K; ++k) {
            binom = binom * (m - k + 1) / k;   /* C(m,k) from C(m,k-1) */
            sum += binom;
        }
        total += (seq_t)phi_cache[d] * sum;
    }

    /* Divide by n – the result is guaranteed integer. */
    return total / n;
}
