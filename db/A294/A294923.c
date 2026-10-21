/*
 * A294923  Number n such that the whole sequence of the first n terms of
 *          A293700 is a palindrome.
 * https://oeis.org/A294923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294923: indices n such that the first n terms of A293700 are a palindrome.
   Method: generate all positive integers i with floor(tan(i))=1, take consecutive
   differences, and keep a rolling 64‑bit hash of the prefix in forward and
   reverse order.  Whenever the two hashes match, the current prefix is a
   palindrome and the current length is recorded.  Results are memoised so
   that a(idx_t n) can be called in arbitrary order. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Prime base for the rolling hash.  Any odd number works; 911382323 is a
   commonly used random 32‑bit prime. */
static const unsigned long long BASE = 911382323ULL;

/* Global state used to generate the sequence lazily. */
static idx_t pal_cnt      = 0;        /* number of palindrome indices found   */
static idx_t diff_cnt     = 0;        /* number of differences processed       */
static idx_t next_i       = 1;        /* next integer to test for tan hit      */
static idx_t prev_hit     = 0;        /* previous i that satisfied floor(tan)=1 */

/* Powers of BASE, used for the reverse hash. */
static unsigned long long *pow_base = NULL;
static size_t pow_cap = 0;

/* Rolling hashes of the prefix in normal and reversed order. */
static unsigned long long forward_hash  = 0;
static unsigned long long backward_hash = 0;

/* Recorded palindrome indices (the answers to the sequence). */
static idx_t *pal_indices = NULL;
static size_t pal_cap     = 0;

/* Ensure that pow_base has at least (idx+1) elements. */
static void ensure_pow_base(size_t idx)
{
    if (pow_cap <= idx) {
        size_t new_cap = pow_cap ? pow_cap * 2 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        unsigned long long *tmp = (unsigned long long *)realloc(pow_base,
                                                  new_cap * sizeof(unsigned long long));
        if (!tmp) abort();
        if (pow_cap == 0) {
            tmp[0] = 1ULL;
            pow_cap = 1;
        }
        for (size_t i = pow_cap; i < new_cap; ++i)
            tmp[i] = tmp[i-1] * BASE;
        pow_base = tmp;
        pow_cap  = new_cap;
    }
}

/* Ensure that pal_indices can hold at least n elements. */
static void ensure_pal_indices(size_t n)
{
    if (pal_cap < n) {
        size_t new_cap = pal_cap ? pal_cap * 2 : 1;
        while (new_cap < n) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(pal_indices, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        pal_indices = tmp;
        pal_cap     = new_cap;
    }
}

/* Main routine requested by the test harness. */
seq_t a(idx_t n)
{
    /* Keep generating until we have found at least n palindrome positions. */
    while (pal_cnt < n) {
        /* Scan integers until we find the next i with floor(tan(i)) == 1. */
        for (;;) {
            double t = tan((double)next_i);
            if (t >= 1.0 && t < 2.0) {            /* hit found */
                if (prev_hit == 0) {              /* first hit */
                    prev_hit = next_i;
                } else {                           /* compute difference */
                    idx_t diff = next_i - prev_hit;
                    prev_hit = next_i;

                    /* Update the rolling hashes. */
                    ensure_pow_base((size_t)diff_cnt);
                    forward_hash  = forward_hash  * BASE + diff;
                    backward_hash += diff * pow_base[(size_t)diff_cnt];

                    ++diff_cnt;                     /* one more term added */

                    /* Check for palindrome using the two hashes. */
                    if (forward_hash == backward_hash) {
                        ensure_pal_indices(pal_cnt + 1);
                        pal_indices[pal_cnt++] = diff_cnt;
                    }
                }
                ++next_i;
                break;
            }
            ++next_i;
        }
    }
    return (seq_t)pal_indices[n-1];
}
