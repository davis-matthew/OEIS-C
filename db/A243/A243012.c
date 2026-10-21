/*
 * A243012  Odd primes p such that neither p - 4 nor p + 4 is prime.
 * https://oeis.org/A243012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes OEIS A243012 – odd primes p such that
 *            neither p‑4 nor p+4 is prime
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of found terms */
    static size_t cap     = 0;        /* capacity of cache array   */
    static size_t count   = 0;        /* number of terms stored    */
    static uint64_t next_candidate = 5; /* start searching from 5     */

    /* n is 1‑based; if n==0 we return 0 (unused, but safe). */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Find the next odd prime. */
        uint64_t p = next_candidate;
        while (!is_prime(p))
            p += 2;

        /* Test the conditions. */
        bool ok = true;
        uint64_t a4 = p - 4;
        if (a4 >= 2 && is_prime(a4))
            ok = false;
        if (ok) {
            uint64_t b4 = p + 4;
            if (is_prime(b4))
                ok = false;
        }

        /* Store if the prime satisfies the condition. */
        if (ok) {
            if (count + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed – terminate */
                cache = tmp;
                cap   = newcap;
            }
            cache[count++] = (seq_t)p;
        }

        /* Update the candidate for the next search – always an odd number. */
        next_candidate = p + 2;
    }

    return cache[n - 1];
}
