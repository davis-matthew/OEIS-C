/*
 * A258084  Numbers k such that k concatenated with its reversal k' yields a
 *          prime when the rightmost digit of k and leftmost digit of k' are
 *          coalesced.
 * https://oeis.org/A258084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k is the n‑th integer (k ≥ 1) satisfying * (k/10)
 *            * 10^{digit_len(rev(k))} + rev(k) is an odd prime. * The
 *            rightmost digit of k is dropped and the reversed string of k *
 *            (with leading zeros removed) is appended; the resulting number
 *            is * a prime. For example, * k = 124 → rev(k) = 4...
 * Range:     u8 n <= 9, u16 n <= 524, u32 n <= 57, u64 n <= 5493,
 *            u128 n <= 5493, big any n
 * Helpers:   digit_len, free_seq_array, is_prime_big, pow10, rev,
 *            seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/rev.h"               /* rev(idx_t) */
#include "../utils/digit_len.h"          /* digit_len(seq_t) */
#include "../utils/pow10.h"              /* pow10(idx_t) */
#include "../utils/is_prime_big.h"       /* is_prime_big(seq_t) */
#include "../utils/seq_alloc_new_array.h"/* seq_alloc_new_array(size_t) */
#include "../utils/free_seq_array.h"     /* free_seq_array(seq_t*) */

static seq_t *cache = NULL;     /* cache of found sequence terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_k = 0;         /* last tested k value */

/* Ensure that 'cache' has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache != NULL)
        free_seq_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* safety for out‑of‑bounds calls */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        idx_t k = last_k + 1;   /* next candidate to test */

        for (;;) {
            idx_t rev_k = rev(k);                     /* reversed k, with leading zeros removed */
            seq_t rev_seq = (seq_t)rev_k;
            int rev_digits = digit_len(rev_seq);      /* number of decimal digits of rev_k */

            seq_t first_part = (seq_t)(k / 10);       /* drop the rightmost digit of k */
            seq_t pow10_rev = pow10((idx_t)rev_digits);
            seq_t candidate = first_part * pow10_rev + rev_seq;

            if (is_prime_big(candidate)) {
                /* Found the next term.  Cache it and break. */
                last_k = k;
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
                break;
            }
            ++k;   /* try next k */
        }
    }

    return cache[n - 1];
}
