/*
 * A364190  The sum of the digits present in a(n) and a(n+1) divides the
 *          product [a(n)*a(n+1)]. This is the lexicographically earliest
 *          sequence of distinct positive terms with this property.
 * https://oeis.org/A364190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest distinct positive sequence such that for all n,
   digit_sum(a(n)) + digit_sum(a(n+1)) divides a(n) * a(n+1).
   Initial term: a(1) = 1. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static storage for the sequence and used numbers */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 for safety */

    /* Build the sequence up to the requested index */
    while (seq_len < n)
    {
        /* Initialise the first term a(1) = 1 */
        if (seq_len == 0)
        {
            /* Allocate sequence storage */
            if (seq_cap == 0) {
                seq_cap = 4;
                seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
            }
            seq[0] = (seq_t)1;
            seq_len = 1;

            /* Allocate used‑array storage */
            if (used_cap == 0) {
                used_cap = 4;
                used = (bool*)calloc(used_cap, sizeof(bool));
            }
            /* Ensure index 1 exists */
            if (1 >= used_cap) {
                size_t new_cap = used_cap * 2;
                if (new_cap <= 1) new_cap = 2;
                used = (bool*)realloc(used, new_cap * sizeof(bool));
                for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
                used_cap = new_cap;
            }
            used[1] = true;
            continue;
        }

        seq_t prev = seq[seq_len - 1];

        /* Find the smallest unused positive integer that satisfies the
           divisibility condition with prev. */
        size_t candidate = 1;
        for (;; ++candidate) {
            /* Grow the used array if necessary */
            if (candidate >= used_cap) {
                size_t new_cap = used_cap * 2;
                if (new_cap <= candidate) new_cap = candidate + 1;
                used = (bool*)realloc(used, new_cap * sizeof(bool));
                for (size_t i = used_cap; i < new_cap; ++i) used[i] = false;
                used_cap = new_cap;
            }
            if (used[candidate]) continue;   /* already in the sequence */

            /* Compute digit sums */
            seq_t sum_prev   = digit_sum(prev);
            seq_t sum_prime  = digit_sum((seq_t)candidate);
            seq_t sum        = sum_prev + sum_prime;
            /* Check divisibility: (prev * candidate) % sum == 0 */
            seq_t rem = ((prev % sum) * ((seq_t)candidate % sum)) % sum;
            if (rem == 0) {
                /* Append the candidate to the sequence */
                if (seq_len >= seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    if (new_cap == 0) new_cap = 4;
                    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq[seq_len] = (seq_t)candidate;
                seq_len++;
                used[candidate] = true;
                break;
            }
        }
    }

    return seq[n - 1];
}
