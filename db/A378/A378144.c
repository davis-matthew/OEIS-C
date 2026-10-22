/*
 * A378144  a(n) = P(n) * 2^floor(log_2(prime(n+1))) = A002110(n) *
 *          A000079(A098388(n+1)).
 * https://oeis.org/A378144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002110(n) * 2^floor(log_2(prime(n+1) - 1)) (Largest
 *            power of 2 times the primorial of n that is strictly less than
 *            the next primorial.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_capacity, floor_log2_uint64, get_prime, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/ensure_capacity.h"

/* Cache of primorials: primorials[i] = product of first i primes
   (primorials[0] = 1). */
static seq_t *primorials = NULL;
static size_t prim_cap = 0;      /* allocated capacity */
static size_t prim_len  = 0;      /* number of primorials computed */

/* Make sure primorials are computed up to index n (inclusive). */
static void
ensure_primorials_up_to(idx_t n)
{
    /* Initial allocation */
    if (prim_len == 0) {
        size_t need = (n + 1 > 1) ? n + 1 : 1;
        ensure_capacity(need, &prim_cap, (void**)&primorials, sizeof(seq_t));
        primorials[0] = (seq_t)1;   /* 0 primes product is 1 */
        prim_len = 1;
    }

    /* Grow and compute new primorials if necessary */
    if (prim_len <= n) {
        size_t new_cap = prim_cap;
        while (new_cap <= n) new_cap <<= 1;
        ensure_capacity(new_cap, &prim_cap, (void**)&primorials, sizeof(seq_t));

        /* Compute from the last known primorial up to n */
        for (idx_t i = prim_len; i <= n; ++i) {
            seq_t prime = (seq_t)get_prime(i);          /* i‑th prime */
            primorials[i] = seq_mul(primorials[i-1], prime);
        }
        prim_len = n + 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primorial for n is ready */
    ensure_primorials_up_to(n);

    seq_t prim = primorials[n];
    uint32_t nextp   = get_prime(n + 1);              /* (n+1)-th prime */
    idx_t   e = floor_log2_uint64((idx_t)(nextp - 1)); /* largest 2^e < nextp */
    seq_t pow = pow2(e);

    return seq_mul(prim, pow);
}
