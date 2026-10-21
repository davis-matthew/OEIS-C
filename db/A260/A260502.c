/*
 * A260502  Log_2 of the numerator of det(M) where M is the n X n matrix with
 *          M[i,j] = 1/lcm(i,j).
 * https://oeis.org/A260502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 329, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A260502 – Log₂ of the numerator of det(M), M[i,j] = 1/lcm(i,j)
 *
 * Formula used:
 *   det(M) = (∏_{k=1}^n φ(k)) / (n!)²            (φ = Euler totient)
 *   a(n)   = v₂(∏_{k=1}^n φ(k)) – 2·v₂(n!)
 *   where v₂(x) is the exponent of 2 in the prime factorization of x
 *   and a(n) is set to 0 if the exponent is negative.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return the exponent of 2 in the factorization of a uint64_t value. */
static unsigned v2_uint64(uint64_t x) {
    unsigned c = 0;
    while ((x & 1ULL) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Build φ[1..N] and prefix sums of v₂(φ(k)) up to N. */
static void
build_phi_and_v2_prefix(idx_t N, idx_t **phi_ptr, unsigned **pref_ptr,
                       idx_t *current_size) {
    /* Allocate new arrays */
    idx_t *new_phi = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    unsigned *new_pref = (unsigned *)malloc((N + 1) * sizeof(unsigned));
    if (!new_phi || !new_pref) return; /* allocation failure – silently abort */

    /* Initialize φ[i] = i */
    new_phi[0] = 0;
    for (idx_t i = 1; i <= N; ++i) new_phi[i] = i;

    /* Sieve to compute φ */
    for (idx_t p = 2; p <= N; ++p) {
        if (new_phi[p] == p) {           /* p is prime */
            for (idx_t j = p; j <= N; j += p) {
                new_phi[j] = (new_phi[j] / p) * (p - 1);
            }
        }
    }

    /* Compute prefix sums of v₂(φ(k)) */
    new_pref[0] = 0;
    for (idx_t i = 1; i <= N; ++i) {
        unsigned v2 = v2_uint64((uint64_t)new_phi[i]);
        new_pref[i] = new_pref[i - 1] + v2;
    }

    /* Free old arrays (if any) */
    free(*phi_ptr);
    free(*pref_ptr);

    /* Replace with new arrays */
    *phi_ptr = new_phi;
    *pref_ptr = new_pref;
    *current_size = N;
}

/* Compute exponent of 2 in n! using Legendre’s formula */
static unsigned v2_factorial(idx_t n) {
    unsigned exp = 0;
    idx_t power = 2;
    while (power <= n) {
        exp += n / power;
        power <<= 1;
    }
    return exp;
}

seq_t a(idx_t n) {
    /* Static cached data */
    static idx_t *phi = NULL;
    static unsigned *pref_v2 = NULL;
    static idx_t built_up_to = 0;

    /* Build up to n if needed */
    if (n > built_up_to) {
        build_phi_and_v2_prefix(n, &phi, &pref_v2, &built_up_to);
    }

    /* Compute exponent of 2 in numerator after reduction */
    unsigned v2_phi_prod = pref_v2[n];
    unsigned v2_nfact = v2_factorial(n);
    int diff = (int)v2_phi_prod - 2 * (int)v2_nfact;
    if (diff < 0) diff = 0;

    return (seq_t)diff;
}
