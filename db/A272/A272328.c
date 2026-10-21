/*
 * A272328  Number of integers 1<=k<=n such that phi(n)=phi(n+k) where phi is
 *          Euler's totient function A000010.
 * https://oeis.org/A272328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A272328
 *  a(n) = |{ 1 ≤ k ≤ n  :  φ(n) = φ(n + k) }|
 *  φ(m) is Euler's totient function.
 *
 *  Implementation:  For each needed n compute φ up to 2n with a
 *  simple sieve, cache the values, and count matching k.
 *  The cache is lazily grown; recomputing from scratch on
 *  each expansion is inexpensive for n ≤ 10 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache of φ values */
static seq_t *phi_cache = NULL;   /* array of φ(i) for 0 ≤ i ≤ φ_cap */
static idx_t  phi_cap   = 0;      /* current maximum index stored */

/* recompute φ[0..limit] using the standard sieve */
static void compute_phi_up_to(idx_t limit)
{
    /* allocate new array */
    seq_t *p = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!p) exit(EXIT_FAILURE);        /* fatal error */

    /* initialise φ[i] = i */
    for (idx_t i = 0; i <= limit; ++i)
        p[i] = (seq_t)i;

    /* apply Euler's product formula for each prime p */
    for (idx_t pprime = 2; pprime <= limit; ++pprime) {
        if (p[pprime] == (seq_t)pprime) {          /* pprime is prime */
            for (idx_t j = pprime; j <= limit; j += pprime)
                p[j] -= p[j] / (seq_t)pprime;
        }
    }

    /* free old cache and replace it */
    free(phi_cache);
    phi_cache = p;
    phi_cap   = limit;
}

/* ensure that φ values up to 'needed' are available */
static void ensure_phi_capacity(idx_t needed)
{
    if (needed <= phi_cap) return;          /* already sufficient */
    compute_phi_up_to(needed);
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety for index 0 */

    /* we need φ up to 2n to test k = 1 … n */
    idx_t max_needed = 2 * n;
    ensure_phi_capacity(max_needed);

    seq_t target = phi_cache[n];
    seq_t count  = 0;

    for (idx_t k = 1; k <= n; ++k)
        if (target == phi_cache[n + k])
            ++count;

    return count;
}
