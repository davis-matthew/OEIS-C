/*
 * A341902  Least k > 1 such that (n^3+k)/(n+k) is an integer.
 * https://oeis.org/A341902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2515, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the smallest k > 1 such that
   (n^3 + k) is divisible by (n + k).  This implements the
   definition of A341902. */

#include <stdint.h>
#include <stdbool.h>

/* Return the sequence value a(n).  All operations are performed
   in seq_t, with n cast to seq_t once at the start of the
   function.  The search stops as soon as a divisor is found. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t once */
    seq_t k_min = (seq_t)UINT64_MAX;

    /* Start with k = 2 (the problem requires k > 1). */
    for (uint64_t k = 2; ; ++k)
    {
        seq_t k_seq      = (seq_t)k;
        seq_t denominator = n_seq + k_seq;

        /* Compute n^3 + k.  Multiplications are performed in seq_t. */
        seq_t n_cube = n_seq * n_seq * n_seq;
        seq_t numer  = n_cube + k_seq;

        /* Check divisibility.  % is allowed for seq_t. */
        if (numer % denominator == 0)
        {
            k_min = k_seq;
            break;
        }
    }
    return k_min;
}
