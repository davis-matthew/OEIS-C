/*
 * A327169  Number of distinct k such that A000005(k)*A000010(k) is equal to
 *          n.
 * https://oeis.org/A327169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} [φ(k)·τ(k) == n]
 * Range:     u8 n <= 103, u16 n <= 9289, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(k) = Euler's totient (A000010), τ(k) = divisor count (A000005) */

#include <stdlib.h>
#include <string.h>   /* for memset */

/* static state for incremental precomputation */
static idx_t Nmax = 0;          /* largest n already precomputed */
static seq_t *phi = NULL;       /* φ(0..Nmax)  */
static seq_t *tau = NULL;       /* τ(0..Nmax)  */

static void ensure_up_to(idx_t new_max)
{
    if (new_max <= Nmax)
        return;              /* already sufficient */

    /* allocate fresh arrays (zeroed) */
    seq_t *new_phi = (seq_t *)calloc((size_t)new_max + 1, sizeof(seq_t));
    seq_t *new_tau = (seq_t *)calloc((size_t)new_max + 1, sizeof(seq_t));
    if (!new_phi || !new_tau) {                /* allocation failure */
        free(new_phi);
        free(new_tau);
        return;
    }

    /* ----- compute φ (Euler totient) ----- */
    /* initialise φ[i] = i for i ≥ 1 */
    for (idx_t i = 1; i <= new_max; ++i)
        new_phi[i] = (seq_t)i;
    new_phi[0] = 0;      /* φ(0) unused but set to 0 */

    /* sieve on primes : if new_phi[p]==p then p is prime */
    for (idx_t p = 2; p <= new_max; ++p) {
        if (new_phi[p] == (seq_t)p) {          /* p is prime */
            for (idx_t j = p; j <= new_max; j += p) {
                new_phi[j] -= new_phi[j] / (seq_t)p;
            }
        }
    }

    /* ----- compute τ (divisor count) ----- */
    /* count all divisors by iterating over multiples */
    for (idx_t i = 1; i <= new_max; ++i) {
        for (idx_t j = i; j <= new_max; j += i) {
            ++new_tau[j];
        }
    }

    /* replace old tables */
    if (phi) free(phi);
    if (tau) free(tau);
    phi = new_phi;
    tau = new_tau;
    Nmax = new_max;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1 */

    ensure_up_to(n);

    seq_t count = 0;
    seq_t target = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t prod = phi[k] * tau[k];
        if (prod == target)
            ++count;
    }
    return count;
}
