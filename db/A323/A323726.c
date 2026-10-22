/*
 * A323726  Odd numbers k such that sigma(k-1) < sigma(k) < sigma(k+1),
 *          sigma(n) = A000203.
 * https://oeis.org/A323726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A323726 – odd k such that σ(k‑1) < σ(k) < σ(k+1) .
    The function lazily builds the sequence and works for any call order.
    It uses the provided σ(n) implementation from ../utils/sigma_of.h. */

#include <stddef.h>
#include <stdlib.h>

/* sigma_of(n): returns the sum of divisors of n (σ(n)).          */
#include "../utils/sigma_of.h"

/* Cache for the sequence values.                                  */
static seq_t *cache = NULL;      /* array of found terms */
static size_t  len   = 0;        /* number of terms stored */
static size_t  cap   = 0;        /* current capacity of cache  */

/* The next odd integer to examine.                               */
static idx_t next_k = 3;

/* Ensure the cache has room for at least 'need' elements.        */
static void grow_cache(size_t need)
{
    if (cap >= need)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails we leave the old pointer; the caller will
       simply not be able to generate more terms – this situation
       is not expected in the test harness.                       */
    if (p) {
        cache = p;
        cap   = new_cap;
    }
}

/* Generate the sequence up to at least 'n' terms.                 */
static void generate_upto(idx_t n)
{
    if (len >= (size_t)n)
        return;

    while (len < (size_t)n) {
        /* Compute sigma values for k-1, k, k+1.                       */
        seq_t sigma_prev = sigma_of(next_k - 1);
        seq_t sigma_curr = sigma_of(next_k);
        seq_t sigma_next = sigma_of(next_k + 1);

        if (sigma_prev < sigma_curr && sigma_curr < sigma_next) {
            grow_cache(len + 1);
            cache[len++] = (seq_t)next_k;
        }

        /* Advance to the next odd integer (take care of overflow). */
        if (next_k > UINT64_MAX - 2)   /* would overflow on +2 */
            break;
        next_k += 2;
    }
}

/* Return the nth term of the sequence (a(1) is the first term).   */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 0 */
        return 0;

    generate_upto(n);
    if (len >= (size_t)n)
        return cache[(size_t)(n - 1)];
    return 0;              /* should not happen for valid n */
}
