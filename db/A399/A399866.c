/*
 * A399866  a(1) = 1; a(n+1) is the smallest integer greater than a(n) such
 *          that: a(k) + a(k+3) is not a term for any k.
 * https://oeis.org/A399866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest integer > a(n-1) such that for all k with k+3 ≤ n,
   the sum a(k)+a(k+3) is not a term of the sequence. */
/* This implementation uses a fixed-size cache (20000 entries),
   sufficient for all n ≤ 10000 as required.  It works for both
   C (integer seq_t) and C++ (arbitrary-precision seq_t) builds. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static cache for the sequence terms */
#define SEQ_CACHE_CAP 20000
static seq_t seq_cache[SEQ_CACHE_CAP];
static idx_t seq_size = 0;   /* how many terms are currently stored */

/* Add the next term to the cache.  The search starts from 1 larger
   than the current largest term and increases until it meets the
   condition that no sum a(k)+a(k+3) equals the candidate. */
static void add_next_term(void)
{
    if (seq_size == 0) {              /* initialise the first four values */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)2;
        seq_cache[2] = (seq_t)3;
        seq_cache[3] = (seq_t)4;
        seq_size = 4;
        return;
    }

    /* candidate always at least one larger than the last stored term */
    seq_t candidate = seq_cache[seq_size - 1] + (seq_t)1;
    bool valid;

    do {
        valid = true;
        for (idx_t j = 0; (j + (idx_t)3) < seq_size; ++j) {
            /* if candidate equals a(j)+a(j+3), it is invalid */
            if (seq_cache[j] + seq_cache[j + 3] == candidate) {
                valid = false;
                break;
            }
        }
        if (!valid)
            ++candidate;
    } while (!valid);

    /* store the found candidate */
    seq_cache[seq_size++] = candidate;
}

/* Public function: return the n‑th term of the sequence.
   Calls are arbitrary; the cache is built lazily. */
seq_t a(idx_t n)
{
    if (n == 0) abort();               /* offset starts at 1 */
    while (seq_size < n)
        add_next_term();
    return seq_cache[n - 1];
}
