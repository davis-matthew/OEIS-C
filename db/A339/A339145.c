/*
 * A339145  a(n) is 1 if A196202(n) is a local maximum, -1 if A196202(n) is a
 *          local minimum and 0 otherwise.
 * https://oeis.org/A339145
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sign of local extremum of A196202(n) = 2^(p-1) mod p^2 (p
 *            is the n‑th prime). 1 for a local maximum, -1 for a local
 *            minimum, 0 otherwise. a(2) is the first term
 * Range:     i8 n <= 6, i16 n <= 42, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* Sequence offset is 2; values below are undefined. */
    if (n < 2)
        return 0;

    /* Retrieve the required primes.  All indices are valid for n >= 2. */
    uint32_t p_prev = get_prime(n - 1);
    uint32_t p_curr = get_prime(n);
    uint32_t p_next = get_prime(n + 1);

    /* Compute A196202 for each prime: 2^(p-1) mod p^2. */
    seq_t mod_prev = (seq_t)p_prev * p_prev;
    seq_t val_prev = mod_pow((seq_t)2, (uint64_t)p_prev - 1, mod_prev);

    seq_t mod_curr = (seq_t)p_curr * p_curr;
    seq_t val_curr = mod_pow((seq_t)2, (uint64_t)p_curr - 1, mod_curr);

    seq_t mod_next = (seq_t)p_next * p_next;
    seq_t val_next = mod_pow((seq_t)2, (uint64_t)p_next - 1, mod_next);

    /* Determine if val_curr is a strict local maximum or minimum. */
    if (val_curr > val_prev && val_curr > val_next)
        return 1;    /* local maximum */
    if (val_curr < val_prev && val_curr < val_next)
        return -1;   /* local minimum */
    return 0;        /* otherwise */
}
