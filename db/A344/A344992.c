/*
 * A344992  a(n) = Sum_{1 <= i <= j <= k <= m <= n} gcd(i,j,k,m).
 * https://oeis.org/A344992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k) * C(floor(n/k)+3,4) (phi: Euler
 *            totient, C: binomial) See OEIS A344992 for details
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 550, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_floorplus3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_floorplus3.h"

/* ----------------------------------------------------------------------
   Simple totient sieve with memoization.
   The array `phi_cache` holds phi(i) for 0 <= i < phi_cap.
   `ensure_phi_cache(n)` expands the array to size n+1 and recomputes
   all phi values from scratch on the new range.
   ---------------------------------------------------------------------- */
static seq_t *phi_cache = NULL;
static size_t phi_cap = 0;

/* Ensure that phi_cache has correct values for 0..n inclusive.
   Recomputes phi for all numbers up to new_cap (n+1). */
static void ensure_phi_cache(idx_t n) {
    size_t new_cap = (size_t)n + 1;
    if (new_cap <= phi_cap) return;          /* already at least this large */

    /* Allocate or resize */
    seq_t *new_cache = (seq_t *)realloc(phi_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                  /* allocation failure */
    phi_cache = new_cache;

    /* Initialise each entry to its own value */
    for (size_t i = 0; i < new_cap; ++i) {
        phi_cache[i] = (seq_t)i;
    }

    /* Sieve totients for all numbers in the new range */
    for (idx_t p = 2; p < (idx_t)new_cap; ++p) {
        if (phi_cache[p] == (seq_t)p) {       /* p is prime */
            for (idx_t m = p; m < (idx_t)new_cap; m += p) {
                phi_cache[m] -= phi_cache[m] / (seq_t)p;
            }
        }
    }

    phi_cap = new_cap;
}

/* ----------------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* safety; offset is 1 */

    ensure_phi_cache(n);

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                  /* floor(n/k) */
        seq_t bin = binom_floorplus3(q);
        seq_t phi_val = phi_cache[k];
        /* Use 128-bit multiplication to avoid overflow during the sum */
        __uint128_t term = (__uint128_t)phi_val * (__uint128_t)bin;
        result += (seq_t)term;
    }
    return result;
}
