/*
 * A382255  Heinz number of the partition corresponding to run lengths in the
 *          bits of n.
 * https://oeis.org/A382255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   floor_log2_uint64, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382255: Heinz number of the partition corresponding to run lengths in the
   binary representation of n.  For each run of equal bits of length L
   we compare the run to the L‑th prime (prime(1)=2, prime(2)=3, …) and
   multiply all such primes together.  The function works for any unsigned
   integer type seq_t supplied by the build.
   It uses floor_log2_uint64 from utils to find the most significant bit.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/isPrimeTrial.h"

/* Small primes sufficient for all run lengths that give a value that
   fits into 8‑bit seq_t (i.e. up to the 20th prime, 71).  For larger
   run lengths we compute the required prime on the fly. */
static const seq_t SMALL_PRIMES[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

/* Return the nth prime (1‑based).  For n <= 20 a lookup is used.
   For larger n the nth prime is generated via trial division. */
static seq_t get_prime_of_len(idx_t n)
{
    if (n <= 20u) {
        return SMALL_PRIMES[n - 1];
    }

    /* Generate primes until we reach the nth one. */
    idx_t count = 20u;
    seq_t candidate = 73;          /* the 21st prime */
    while (count < n) {
        if (isPrimeTrial(candidate)) {
            ++count;
        }
        candidate += 2;
    }
    return candidate - 2;          /* last prime found */
}

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;           /* empty product */
    }

    idx_t msb = floor_log2_uint64(n);
    seq_t result = (seq_t)1;

    int cur_bit = (int)((n >> msb) & (idx_t)1);
    idx_t run = (idx_t)1;

    for (idx_t i = msb; i > 0u; --i) {
        int next_bit = (int)((n >> (i - 1u)) & (idx_t)1);
        if (next_bit == cur_bit) {
            ++run;
        } else {
            result *= get_prime_of_len(run);
            run = (idx_t)1;
            cur_bit = next_bit;
        }
    }

    /* Multiply the last run. */
    result *= get_prime_of_len(run);
    return result;
}
