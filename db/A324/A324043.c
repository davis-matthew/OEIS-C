/*
 * A324043  Number of quadrilateral regions into which a figure made up of a
 *          row of n adjacent congruent rectangles is divided upon drawing
 *          diagonals of all possible rectangles.
 * https://oeis.org/A324043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2(n-1)^2 + Sum_{i=2..floor(n/2)} (n+1-i)*(7i-2n-2)*φ(i)
 *            + Sum_{i=floor(n/2)+1..n} (n+1-i)*(2n+2-i)*φ(i) φ(i) is Euler's
 *            totient
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an incremental sieve to cache φ values up to the
   largest n requested.  The cache is updated only when a larger n is
   requested, so calls in non‑monotonic order remain inexpensive.  */

#include <stdlib.h>
#include <stdint.h>

/* Static totient cache.
   phi_cache[1] = 1, phi_cache[i] = φ(i) for 2 ≤ i ≤ phi_cached_max. */
static uint64_t *phi_cache = NULL;
static idx_t phi_cached_max = 0;

/* Ensure the totient cache contains values up to n.  */
static void ensure_phi(idx_t n)
{
    if (n <= phi_cached_max)
        return;                     /* already up to date */

    /* Resize cache.  Cast realloc result for C++ compatibility. */
    uint64_t *tmp = (uint64_t *)realloc(phi_cache, (n + 1) * sizeof(uint64_t));
    if (!tmp)
        exit(1); /* out of memory */
    phi_cache = tmp;

    /* Initialise φ(i)=i for 1 ≤ i ≤ n.  */
    for (idx_t i = 1; i <= n; ++i)
        phi_cache[i] = i;

    /* Euler's sieve.  */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == p) {   /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi_cache[j] -= phi_cache[j] / p;
        }
    }
    phi_cache[1] = 1;              /* φ(1)=1 */

    phi_cached_max = n;
}

/* Sequence function.  */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                    /* a(1) = 0 */

    ensure_phi(n);                  /* φ up to n */

    /* Use signed 64‑bit arithmetic for intermediate sums; this is safe
       for n ≤ 10 000 (max term ≈ 1e16) and keeps the code simple. */
    int64_t sum = -2LL * (int64_t)(n - 1) * (int64_t)(n - 1);

    idx_t half = n / 2;

    /* First sum: i = 2 .. floor(n/2) */
    for (idx_t i = 2; i <= half; ++i) {
        int64_t sign_term = (int64_t)(7 * i - 2 * n - 2);
        int64_t term = (int64_t)(n + 1 - i) * sign_term * (int64_t)phi_cache[i];
        sum += term;
    }

    /* Second sum: i = floor(n/2)+1 .. n */
    for (idx_t i = half + 1; i <= n; ++i) {
        int64_t term = (int64_t)(n + 1 - i) *
                       (int64_t)(2 * n + 2 - i) *
                       (int64_t)phi_cache[i];
        sum += term;
    }

    return (seq_t)sum;
}
