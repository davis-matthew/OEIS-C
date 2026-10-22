/*
 * A378641  Largest m <= n such that phi(m) does not divide n, or -1 if no
 *          such m exists, where phi is the Euler totient function (A000010).
 * https://oeis.org/A378641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = largest m ≤ n such that phi(m) does not divide n; -1 if
 *            none
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *phi_cache = NULL;
static idx_t phi_cap = 0;

/* Compute Euler's totient φ(i) for 1 ≤ i ≤ n, storing in phi_cache */
static void
ensure_phi(idx_t n)
{
    if (n + 1 <= phi_cap)
        return;                     /* already computed up to at least n */

    /* Allocate new cache and free the old one */
    free(phi_cache);
    size_t size = (size_t)(n + 1);
    phi_cache = (seq_t *)malloc(size * sizeof(seq_t));
    if (!phi_cache)
        return;                     /* allocation failed – caller will handle */

    /* Initialize φ(i) = i */
    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (seq_t)i;

    /* Sieve to compute φ(i) */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (phi_cache[i] == (seq_t)i)          /* i is prime */
        {
            for (idx_t j = i; j <= n; j += i)
                phi_cache[j] -= phi_cache[j] / (seq_t)i;
        }
    }

    phi_cap = (idx_t)(n + 1);
}

seq_t
a(idx_t n)
{
    /* For n = 1 or 2, no such m exists */
    if (n < 3)
        return (seq_t)-1;

    /* Odd n ≥ 3 always satisfy the condition with m = n */
    if ((n & (idx_t)1))
        return (seq_t)n;

    /* Ensure φ(m) values are available */
    ensure_phi(n);

    for (idx_t m = n; m >= 1; --m)
    {
        seq_t ph = phi_cache[m];
        idx_t ph_idx = (idx_t)ph;
        if (ph_idx == 0)
            continue;                 /* safety, should not happen */
        if (n % ph_idx != 0)
            return (seq_t)m;          /* largest m found */
    }

    /* No m found */
    return (seq_t)-1;
}
