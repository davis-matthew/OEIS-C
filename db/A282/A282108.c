/*
 * A282108  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j+1..k}{(i-j)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 3.
 * https://oeis.org/A282108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 7210, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generates terms sequentially by brute-force search
   that checks the fulcrum condition in base 3. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static state for memoization */
static seq_t *seq       = NULL; /* array of computed terms */
static size_t seq_cap   = 0;    /* capacity of seq array   */
static size_t seq_len   = 0;    /* current number of terms */
static seq_t  last_candidate = 1; /* next candidate to test */

/* Append a term to the memoization table, growing if necessary */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1U; /* double until enough */
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Test whether a given number satisfies the fulcrum condition.
   The number is already in base‑3 representation in its natural
   order with least significant digit first. */
static bool satisfies(seq_t n)
{
    /* Extract base‑3 digits */
    seq_t tmp = n;
    unsigned char digits[128];   /* enough for the sizes we need */
    size_t k = 0;                /* number of digits */

    while (tmp != 0) {
        digits[k++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }
    if (k < 3)      /* need at least one digit on each side of the fulcrum */
        return false;

    /* Test each possible fulcrum position j (1 … k‑2, 0‑based) */
    for (size_t j = 1; j + 1 < k; ++j) {
        seq_t left  = 0;
        seq_t right = 0;

        /* Left side: i < j  (weights j-i) */
        for (size_t i = 0; i < j; ++i)
            left += (seq_t)(j - i) * digits[i];

        /* Right side: i > j (weights i-j) */
        for (size_t i = j + 1; i < k; ++i)
            right += (seq_t)(i - j) * digits[i];

        if (left == right)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n = 0, but protect anyway */
        return 0;

    /* Extend memoization table if necessary */
    while (seq_len < (size_t)n) {
        /* Find the next candidate that satisfies the condition */
        while (true) {
            if (satisfies(last_candidate)) {
                /* Store the found term */
                grow_seq(seq_len + 1);
                seq[seq_len++] = last_candidate;
                break;
            }
            ++last_candidate;
        }
        /* Continue searching from the number after the one just added */
        ++last_candidate;
    }

    /* Return the (n‑1)th element (1‑based indexing) */
    return seq[(size_t)n - 1];
}
