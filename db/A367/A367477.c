/*
 * A367477  a(n) is the least k such that all possible modular classes a
 *          Fibonacci number can take mod n is seen in the Fibonacci numbers
 *          Fibonacci(1)..Fibonacci(k).
 * https://oeis.org/A367477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A367477:  a(n) = least k such that the Fibonacci numbers
   Fibonacci(1)..Fibonacci(k) contain all residues that appear in the
   Fibonacci sequence modulo n.  The algorithm generates one full Pisano
   period of Fibonacci numbers modulo n, determines the set of residues
   that occur, and then finds the maximum index of their first
   occurrence.  The Pisano period is at most 6*n (for n>1); for n==1
   the answer is trivially 1. */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special‑case n==1: all residues are 0, first occurs at index 1. */
    if (n == 1) return (seq_t)1;

    /* Convert n to the arithmetic type used for residues. */
    seq_t mod = (seq_t)n;

    /* Allocate storage for the residues of one full period.
       Pisano period ≤ 6*n for n>1. */
    size_t capacity = 6 * (size_t)n + 5;
    seq_t *residues = (seq_t*)malloc(sizeof(seq_t) * capacity);
    if (!residues) return 0;          /* allocation failed */

    /* Boolean arrays to record which residues appear at all and
       which ones have been seen while scanning. */
    bool *possible = (bool*)calloc((size_t)n, sizeof(bool));
    bool *seen     = (bool*)calloc((size_t)n, sizeof(bool));
    if (!possible || !seen) { free(residues); return 0; }

    /* Generate Fibonacci numbers modulo n until the pair (0,1) repeats. */
    seq_t prev = 0;
    seq_t curr = (seq_t)1 % mod;      /* Fibonacci(1) */
    size_t period_len = 0;

    while (true)
    {
        /* Record the current residue. */
        residues[period_len++] = curr;

        /* Advance to the next Fibonacci number modulo n. */
        seq_t next = (prev + curr) % mod;
        prev = curr;
        curr = next;

        /* Terminate after the period recommences. */
        if (prev == 0 && curr == 1)
            break;
    }

    /* Mark which residues actually appear in the period. */
    for (size_t i = 0; i < period_len; ++i)
    {
        size_t r = (size_t)residues[i];
        if (!possible[r]) possible[r] = true;
    }

    /* Find the minimal k: the largest first‑occurrence index among all
       residues that appear. */
    seq_t max_k = 0;
    for (size_t i = 0; i < period_len; ++i)
    {
        size_t r = (size_t)residues[i];
        if (possible[r] && !seen[r])
        {
            seen[r] = true;
            seq_t idx = (seq_t)(i + 1);   /* indices are 1‑based */
            if (idx > max_k) max_k = idx;
        }
    }

    /* Clean up. */
    free(residues);
    free(possible);
    free(seen);

    return max_k;
}
