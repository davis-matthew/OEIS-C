/*
 * A336224  Numbers k such that the largest square dividing k is a unitary
 *          divisor of k and its square root has an even number of prime
 *          divisors (counted with multiplicity).
 * https://oeis.org/A336224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336224: numbers k whose largest square divisor is a unitary divisor of k
   and whose square root has an even number of prime divisors (counted with multiplicity).
   Computed by a simple sieve-like search: factor each k, check that all prime exponents
   are either 1 or even, and that the sum of e/2 over even e is even. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* Determine whether k is in the sequence. */
static bool in_seq(uint64_t k)
{
    if (k == 1) return true;          /* special case */
    uint64_t n = k;
    int parity = 0;                   /* parity of sum of (e/2) over even exponents */

    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        if ((e & 1) && e > 1)          /* odd exponent > 1 fails unitary condition */
            return false;

        if ((e & 1) == 0) {           /* e is even */
            if (((e >> 1) & 1) != 0)    /* e/2 is odd -> toggle parity */
                parity ^= 1;
        }
    }

    return parity == 0;               /* need even number of prime factors in sqrt(sqmax) */
}

/* Dynamically grow the cache of sequence terms. */
static void grow_cache(size_t needed, size_t *cap, void **ptr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    void *tmp = realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure */
    *ptr = tmp;
    *cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static uint64_t next_k = 1;

    if (n == 0) return 0;  /* undefined, but avoid UB */

    while ((idx_t)cache_len < n) {
        if (in_seq(next_k))
            grow_cache(cache_len + 1, &cache_cap, (void **)&cache),
            cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
