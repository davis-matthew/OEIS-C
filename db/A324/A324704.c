/*
 * A324704  Lexicographically earliest sequence containing 1 and all numbers >
 *          2 divisible by prime(m) for some m already in the sequence.
 * https://oeis.org/A324704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes the lexicographically earliest sequence containing 1
 *            and all numbers > 2 divisible by prime(m) for some m already in
 *            the sequence, by incremental construction
 * Range:     u8 n <= 59, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *allowed = NULL;
    static size_t allowed_len = 0, allowed_cap = 0;

    if (!initialized)
    {
        /* Initialize sequence array with a(1) = 1 */
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;

        /* Start with allowed primes: prime(1) = 2 */
        allowed_cap = 8;
        allowed = (seq_t *)malloc(allowed_cap * sizeof(seq_t));
        allowed[0] = get_prime((idx_t)1);  /* 2 */
        allowed_len = 1;

        initialized = true;
    }

    while (seq_len < n)
    {
        seq_t last = seq[seq_len - 1];
        seq_t candidate = last + (seq_t)1;
        if (candidate <= (seq_t)2) candidate = (seq_t)3;

        /* Find next number divisible by any allowed prime */
        for (;;)
        {
            bool ok = false;
            for (size_t i = 0; i < allowed_len; ++i)
            {
                if (candidate % allowed[i] == (seq_t)0)
                {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
            ++candidate;
        }

        /* Append the new term */
        grow_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = candidate;

        /* Add prime(candidate) to allowed set */
        idx_t cand_idx = (idx_t)candidate;
        seq_t new_prime = get_prime(cand_idx);
        grow_array(&allowed, &allowed_cap, allowed_len + 1);
        allowed[allowed_len++] = new_prime;
    }

    return seq[n - 1];   /* a is 1‑indexed */
}
