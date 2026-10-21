/*
 * A276967  Odd integers n such that 2^n == 2^3 (mod n).
 * https://oeis.org/A276967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2549, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates the nth odd integer n such that 2^n ≡ 8 (mod n). */
/* The sequence is defined via a simple search:  for each odd integer n,
   test whether 2^n mod n equals 8 mod n.  The function memoises
   found terms so that calls may arrive in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Memoised array of sequence terms. */
static seq_t *seq_arr = NULL;
static size_t seq_size = 0;   /* number of terms stored */
static size_t seq_cap  = 0;   /* capacity of the array */

/* Next odd integer to test. */
static idx_t next_candidate = 1;

/* Grow the sequence array when capacity is exhausted. */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_t *new_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);   /* unrecoverable error */
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, guard against 0 */

    /* Generate terms until we have enough. */
    while (seq_size < n) {
        seq_t cand = (seq_t)next_candidate;   /* odd integer candidate */

        /* Compute 2^cand modulo cand. */
        seq_t pow_val = mod_pow((seq_t)2, (uint64_t)next_candidate, cand);

        /* RHS is 8 mod cand. */
        seq_t rhs = ((seq_t)8) % cand;

        if (pow_val == rhs) {
            /* Store the candidate as the next sequence term. */
            if (seq_size == seq_cap) grow_seq();
            seq_arr[seq_size++] = cand;
        }

        /* Advance to the next odd integer. */
        next_candidate += 2;
    }

    return seq_arr[n - 1];
}
