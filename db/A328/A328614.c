/*
 * A328614  Number of 1-digits in primorial base expansion of n.
 * https://oeis.org/A328614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of 1‑digits in the primorial base expansion of
 *            n
 * Range:     u8 n <= 2318, u16 n <= 9740, u32 n <= 9740, u64 n <= 9740,
 *            u128 n <= 9740, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *primorial_vals = NULL;  /* primorial[0] = 1, primorial[i] = product of first i primes */
static size_t prim_cap   = 0;          /* allocated size */
static size_t prim_len   = 0;          /* number of primorial entries */

/* Ensure primorial_vals contains all primorials ≤ max_n (in idx_t). */
static void ensure_primorial(idx_t max_n)
{
    /* Grow until the last primorial exceeds max_n or we have at least one entry. */
    while (prim_len == 0 || primorial_vals[prim_len-1] <= (seq_t)max_n) {
        /* Increase capacity if needed. */
        if (prim_len >= prim_cap) {
            size_t new_cap = prim_cap ? prim_cap * 2 : 8;
            seq_t *new_arr;
#ifdef __cplusplus
            new_arr = new seq_t[new_cap];
            for (size_t i = 0; i < prim_len; ++i)
                new_arr[i] = primorial_vals[i];
            delete[] primorial_vals;
#else
            new_arr = realloc(primorial_vals, new_cap * sizeof(seq_t));
            /* realloc keeps existing data */
#endif
            primorial_vals = new_arr;
            prim_cap = new_cap;
        }

        /* Append the next primorial.  primorial_vals[0] = 1. */
        if (prim_len == 0) {
            primorial_vals[0] = (seq_t)1;
        } else {
            uint32_t p = get_prime((idx_t)prim_len);   /* next prime */
            primorial_vals[prim_len] = primorial_vals[prim_len-1] * (seq_t)p;
        }
        ++prim_len;
    }
}

/* Number of 1‑digits in the primorial base expansion of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    ensure_primorial(n);

    /* Find largest k with primorial_vals[k] ≤ n. */
    size_t k = prim_len - 1;
    while (k > 0 && primorial_vals[k] > (seq_t)n)
        --k;

    seq_t remainder = (seq_t)n;
    int count = 0;

    /* Subtract multiples of primorials from highest to lowest (excluding 1). */
    for (; k > 0; --k) {
        seq_t q = remainder / primorial_vals[k];
        if (q == (seq_t)1)
            ++count;
        remainder -= q * primorial_vals[k];
    }

    /* Base unit (prime[0] = 2, primorial[0] = 1).  Count a remaining 1. */
    if (remainder == (seq_t)1)
        ++count;

    return (seq_t)count;
}
