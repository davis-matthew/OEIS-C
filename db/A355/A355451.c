/*
 * A355451  a(n) = 1 if n is even and phi(x) = n has no solutions, otherwise
 *          0.
 * https://oeis.org/A355451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and no integer x satisfies φ(x)=n,
 *            otherwise 0
 * Range:     u8 n <= 9995, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment */

/* Internal static state for the preimage bitmap of totients */
static bool *preimage = NULL;
static idx_t current_limit = 0;

/* Recompute the totient preimage bitmap up to `limit` */
static void recompute_upto(idx_t limit)
{
    /* Allocate a bitmap for values φ(x) ≤ limit */
    bool *new_preimage = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!new_preimage) return;          /* allocation failed – keep old bitmap */

    /* Temporary array to store intermediate φ(x) values */
    idx_t *phi = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!phi) { free(new_preimage); return; }

    /* Initialise φ[i] = i */
    for (idx_t i = 0; i <= limit; ++i) phi[i] = i;

    /* Classic sieve for Euler’s totient function */
    for (idx_t p = 2; p <= limit; ++p) {
        if (phi[p] == p) {                 /* p is prime */
            for (idx_t j = p; j <= limit; j += p)
                phi[j] = phi[j] / p * (p - 1);
        }
    }

    /* Mark every φ(x) that appears (only those ≤ limit matter) */
    for (idx_t i = 1; i <= limit; ++i)
        if (phi[i] <= limit) new_preimage[phi[i]] = true;

    free(phi);

    /* Replace the old bitmap */
    free(preimage);
    preimage = new_preimage;
    current_limit = limit;
}

/* Ensure that the preimage bitmap covers all n up to the requested value. */
static void ensure_limit(idx_t n)
{
    if (current_limit >= n) return;

    /* For the intended range (≈10 000) 200 000 is safe and inexpensive. */
    idx_t new_limit = (n > 200000) ? (n * 2 + 1000) : 200000;
    recompute_upto(new_limit);
}

/* External function required by the OEIS build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 – be safe */

    /* Odd n cannot be the totient of any integer (except 1). */
    if ((n & 1U) != 0) return (seq_t)0;

    ensure_limit(n);

    /* Return 1 if no preimage exists for this even n, else 0. */
    return preimage[n] ? (seq_t)0 : (seq_t)1;
}
