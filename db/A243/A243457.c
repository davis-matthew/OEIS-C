/*
 * A243457  Primes p such that (p reversed)-2 is also a prime.
 * https://oeis.org/A243457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 244, u32 n <= 244, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that reverse(p) - 2 is also prime.
   The function maintains a static cache so that calls in any order
   are answered correctly. The cache is resized using seq_realloc
   to work with both C and C++ arbitrary‑precision builds. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* The offset for this sequence is 1, so n>=1. */
    if (n == 0) return (seq_t)0;

    /* Static cache of found special primes. */
    static seq_t *special = NULL;
    static idx_t count = 0;         /* number of special primes found */
    static idx_t last_checked = 5;  /* next candidate to test */
    static size_t cap = 0;          /* capacity of the cache array */

    /* Extend the cache until it contains at least n elements. */
    if (n > count) {
        idx_t candidate = last_checked;
        while (count < n) {
            /* Skip non‑primes. */
            while (!is_prime_big((seq_t)candidate)) {
                candidate++;
            }
            /* candidate is a prime. */
            seq_t rev = reverse_decimal((seq_t)candidate);
            seq_t rev_minus = rev - (seq_t)2;
            if (rev_minus >= (seq_t)2 && is_prime_big(rev_minus)) {
                /* Store the special prime. */
                if (count >= cap) {
                    size_t newcap = cap ? cap * 2 : 8;
                    special = seq_realloc(special, cap, newcap);
                    cap = newcap;
                }
                special[count++] = (seq_t)candidate;
            }
            candidate++;
            last_checked = candidate;
        }
    }
    return special[n - 1];
}
