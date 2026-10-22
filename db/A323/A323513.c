/*
 * A323513  a(n) = 1 if n and phi(n) share the same prime signature, 0
 *          otherwise.
 * https://oeis.org/A323513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n and φ(n) have the same multiset of * exponents in
 *            their prime factorisations, 0 otherwise. * a(1) = 1 (empty
 *            signature), etc
 * Range:     u8 n <= 2618, u16 n <= 9474, u32 n <= 9474, u64 n <= 9474,
 *            u128 n <= 9474, big any n
 * Helpers:   factorize, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/factorize.h"

/* Simple bubble sort for very small arrays of exponents. */
static void sort_ints(int *a, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        for (size_t j = i + 1; j < len; ++j)
            if (a[i] > a[j]) {
                int t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
}

/* OEIS A323513 – prime‐signature comparison of n and φ(n). */
seq_t a(idx_t n)
{
    if (n < 1)      /* not expected to be called with n==0 */
        return 0;

    /* Factorise n. */
    uint64_t primes_n[64];
    int     exps_n [64];
    size_t  cnt_n = factorize((uint64_t)n, primes_n, exps_n);
    sort_ints(exps_n, cnt_n);

    /* Compute φ(n). */
    seq_t phi_n_seq = phi(n);
    /* φ(n) is known to fit in uint64_t for n up to the limits tested. */
    uint64_t phi_n = (uint64_t)phi_n_seq;

    /* Factorise φ(n). */
    uint64_t primes_phi[64];
    int     exps_phi [64];
    size_t  cnt_phi = factorize(phi_n, primes_phi, exps_phi);
    sort_ints(exps_phi, cnt_phi);

    /* Compare the exponent multisets. */
    if (cnt_n != cnt_phi)
        return 0;
    for (size_t i = 0; i < cnt_n; ++i)
        if (exps_n[i] != exps_phi[i])
            return 0;

    return 1;
}
