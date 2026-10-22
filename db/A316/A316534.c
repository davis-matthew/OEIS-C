/*
 * A316534  Numbers k such that k concatenated with k+1 and then divided by
 *          2k+1 produces an integer after a series of divisions explained in
 *          the Example section.
 * https://oeis.org/A316534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 4546, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   digit_len, pow10, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A316534:
   a(n) is the n‑th integer k (starting from k=1) such that
   the following iterative process eventually finds a step
   where the decimal concatenation of the two current numbers
   is divisible by their sum.
   Process for a given k:
       a = k,   b = k+1
       repeat:
           concat = decimal_concat(a, b)   // e.g. 12 for a=1,b=2
           denom  = a + b
           if concat % denom == 0  // divisible
               success
           a, b = b, floor(concat/denom)
   If success occurs at any step, k is in the sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* ---------- cache for already computed terms -------------------- */
static seq_t *cache = NULL;      /* array of found k's          */
static size_t cache_len = 0;      /* number of terms stored     */
static size_t cache_cap = 0;      /* allocated capacity         */
static idx_t next_k = 1;          /* next candidate to test     */

/* ensure the cache can hold at least 'need' entries                */
static void cache_ensure(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache) abort();                /* allocation failed */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache)
        seq_free_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* test whether a single k satisfies the sequence condition     */
static bool qualifies(idx_t k)
{
    seq_t a = (seq_t)k;          /* current first number   */
    seq_t b = a + 1;             /* current second number  */
    seq_t prev_a = 0, prev_b = 0;

    /* limit iterations to avoid endless loops                  */
    for (int step = 0; step < 500; ++step)
    {
        /* number of decimal digits of b                       */
        int b_digits = digit_len(b);
        if (b_digits < 0)
            return false;       /* unlikely, but safety */

        /* compute 10^b_digits                                      */
        seq_t p10 = pow10((idx_t)b_digits);

        /* concatenation of a and b                                */
        seq_t concat = a * p10 + b;

        /* denominator for division                               */
        seq_t denom = a + b;

        if (concat % denom == 0)            /* divisible -> accept   */
            return true;

        /* detect immediate cycle (no progress)                 */
        if (a == prev_a && b == prev_b)
            break;

        prev_a = a;
        prev_b = b;

        seq_t q = concat / denom;           /* floor division result */
        a = b;
        b = q;
    }
    return false;                            /* failed to find divisor */
}

/* public function: return n-th term (1‑based) of the sequence     */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out‑of‑bounds, return 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* find next qualifying k */
        while (!qualifies(next_k))
            next_k++;

        cache_ensure(cache_len + 1);
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
