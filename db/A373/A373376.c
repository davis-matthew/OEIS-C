/*
 * A373376  Numbers k such that gcd(A001414(k), A003415(k)) is odd, where
 *          A001414 is the sum of prime factors with repetition, and A003415
 *          is the arithmetic derivative.
 * https://oeis.org/A373376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   include_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A373376. Numbers k such that gcd(A001414(k), A003415(k)) is odd.
   Computed by an iterative sieve that checks the parity of the sum of odd
   prime exponents (E) and the exponent of 2 (a). Inclusion rule: keep k
   if (a == 1) or (E is odd). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/include_k.h"

/* Static cache of sequence values (stored as idx_t) */
static idx_t  *seq_vals      = NULL;
static size_t seq_len        = 0;
static size_t seq_cap        = 0;
static idx_t  next_check     = 2;          /* next integer to test */

/* Grow the cache until it contains at least n elements */
static void grow_cache(idx_t n)
{
    if (seq_len >= n) return;

    /* Grow capacity if needed */
    if (seq_cap == 0) seq_cap = 16;
    while (seq_cap < n) seq_cap <<= 1;

    seq_vals = (idx_t*)realloc(seq_vals, seq_cap * sizeof(idx_t));
    if (!seq_vals) exit(EXIT_FAILURE);

    /* Test successive integers and store qualifying ones */
    while (seq_len < n) {
        if (include_k(next_check))
            seq_vals[seq_len++] = next_check;
        next_check++;                       /* advance to next integer */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑bounds */
    grow_cache(n);
    return (seq_t)seq_vals[n - 1];
}
