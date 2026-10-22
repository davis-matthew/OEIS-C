/*
 * A392024  Positive integers k such that gcd(k, tau(k)) is prime.
 * https://oeis.org/A392024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   divisor_count_u64, ensure_capacity, gcd, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392024: Positive integers k such that gcd(k, τ(k)) is prime */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/ensure_capacity.h"
#include "../utils/isPrimeGeneric.h"

/* Returns the nth term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored sequence values   */
    static size_t len   = 0;         /* number of stored terms */
    static size_t cap   = 0;         /* allocated capacity */
    static uint64_t next_k = 2;      /* next integer to test   */

    /* extend cache until it contains at least n terms */
    while (len < n)
    {
        uint64_t k = next_k++;
        uint64_t tau = divisor_count_u64(k);
        seq_t g = gcd((seq_t)k, (seq_t)tau);
        if (isPrimeGeneric(g))
        {
            if (len + 1 > cap)
                ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
