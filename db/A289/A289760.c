/*
 * A289760  Prime powers P for which the number of modulo P residues among
 *          sums of four sixth powers is less than P.
 * https://oeis.org/A289760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A289760.
 * The sequence consists of all prime powers p^k with
 * p ∈ {2,3,7,13,31} except 2^1, 2^2 and 31^1.
 * The powers are listed in increasing order and a(n)
 * returns the n‑th value (1‑based).  The algorithm
 * merges the five increasing power streams by
 * repeatedly taking the smallest current value
 * and advancing that stream by one exponent.  All
 * operations are performed with the libarary function
 * mul_small (base × small integer).  */
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Prime bases and their minimal allowed exponents */
    const seq_t primes[5] = {2, 3, 7, 13, 31};
    const idx_t  min_exp[5] = {3, 2, 1, 1, 2};

    /* Current power and exponent for each prime */
    seq_t cur[5];
    idx_t exp[5];

    /* Initialise each stream to its starting power */
    for (int i = 0; i < 5; ++i) {
        exp[i]   = min_exp[i];
        cur[i]   = 1;
        for (idx_t j = 0; j < exp[i]; ++j)
            cur[i] = mul_small(cur[i], (unsigned)primes[i]);
    }

    seq_t result = 0;

    /* Generate the sequence up to the n‑th element */
    for (idx_t count = 0; count < n; ++count) {
        /* Find the smallest current power */
        int   min_idx = 0;
        seq_t min_val = cur[0];
        for (int i = 1; i < 5; ++i) {
            if (cur[i] < min_val) {
                min_val = cur[i];
                min_idx = i;
            }
        }
        result = min_val;

        /* Advance the selected prime power stream */
        ++exp[min_idx];
        cur[min_idx] = mul_small(cur[min_idx], (unsigned)primes[min_idx]);
    }

    return result;
}
