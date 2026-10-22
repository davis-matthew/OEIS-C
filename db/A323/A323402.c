/*
 * A323402  a(n) = 1 if A023900(n) > 0 and A000010(A023900(n)) ==
 *          A023900(A000010(n)), otherwise a(n) = 0. Here A000010 is Euler
 *          totient function phi, and A023900 is its Dirichlet inverse.
 * https://oeis.org/A323402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A023900(n) > 0 and phi(A023900(n)) ==
 *            A023900(phi(n)), otherwise a(n) = 0. A023900 is the Dirichlet
 *            inverse of Euler's totient function
 * Range:     u8 n <= 6452, u16 n <= 9628, u32 n <= 9628, u64 n <= 9628,
 *            u128 n <= 9628, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */

/* Cache for the Dirichlet inverse of phi.  The values can be negative,
   so we store them in a signed 64‑bit integer. */
static int64_t *a_inv = NULL;
static size_t a_size = 0;      /* current maximum n cached */

/* Build a_inv up to 'limit' (inclusive). */
static void build_to(idx_t limit)
{
    if (limit <= a_size)
        return;  /* already built */

    /* Allocate new array; use explicit casts for C++ compatibility. */
    int64_t *new_a = (int64_t *)calloc(limit + 1, sizeof(int64_t));
    if (!new_a)
        exit(1);     /* allocation failure */

    /* Dirichlet inverse of phi: a(1) = 1. */
    new_a[1] = 1;

    for (idx_t n = 2; n <= limit; ++n) {
        int64_t sum = 0;
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;
                idx_t d2 = n / d;

                /* d1 < n always holds here, so we add its contribution. */
                seq_t phi_d2 = phi(d2);
                sum += (int64_t)new_a[d1] * (int64_t)phi_d2;

                /* d2 is distinct from d1 and < n (since n > d2). */
                if (d2 != d1) {
                    seq_t phi_d1 = phi(d1);
                    sum += (int64_t)new_a[d2] * (int64_t)phi_d1;
                }
            }
        }
        new_a[n] = -sum;
    }

    /* Replace the old cache. */
    free(a_inv);
    a_inv = new_a;
    a_size = limit;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* below the offset; behaviour undefined */

    /* Ensure the cache covers n. */
    if (n > a_size)
        build_to(n);

    int64_t inv_n = a_inv[n];
    if (inv_n <= 0)
        return 0;      /* A023900(n) must be positive */

    seq_t phi_inv_n = phi((idx_t)inv_n);

    /* φ(n) is guaranteed to be <= n, so its inverse is already cached. */
    seq_t phi_n_seq = phi(n);
    idx_t phi_n = (idx_t)phi_n_seq;

    if (phi_n > a_size)
        /* This can only happen if phi(n) == n, which would imply n=1,
           but we already handled n==1 above.  For safety, extend cache. */
        build_to(phi_n);

    int64_t inv_phi_n = a_inv[phi_n];
    if (inv_phi_n <= 0)
        return 0;

    /* Compare φ(A023900(n)) with A023900(φ(n)). */
    return ((seq_t)inv_phi_n == phi_inv_n) ? 1 : 0;
}
