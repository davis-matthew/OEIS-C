/*
 * A318940  Primes whose binary representation contains a consecutive string
 *          of zeros of prime length.
 * https://oeis.org/A318940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 949, u32 n <= 949, u64 n <= 949,
 *            u128 n <= 949, big any n
 * Helpers:   bitlen_seq_t, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose binary representation contains a consecutive
 * string of zeros of prime length (search-based implementation). */
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

/* Return true if n, a prime, has a run of consecutive zeros in its binary
 * representation whose length is a prime number. */
static bool has_prime_zero_run(seq_t n)
{
    /* bitlen_seq_t gives the number of bits needed to represent n */
    unsigned long len = bitlen_seq_t(n);
    unsigned long run = 0;

    for (unsigned long i = 0; i < len; ++i) {
        /* Test the i-th bit (0 = even, 1 = odd).  We shift n right by i
         * and mask with 1; the shift count is an unsigned long, which
         * matches the overload provided by the arbitrary‑precision
         * implementation. */
        if (((n >> i) & 1) == 0) {
            ++run;
        } else {
            if (run > 0 && is_prime_small((seq_t)run))
                return true;
            run = 0;
        }
    }
    /* If the binary ends with a zero run (not possible for odd primes
     * but kept for completeness). */
    if (run > 0 && is_prime_small((seq_t)run))
        return true;
    return false;
}

/* Return the n-th term of A318940 (n ≥ 1).  The function works for
 * arbitrary-precision seq_t and idx_t (uint64_t).  If n == 0 the value
 * 0 is returned, but all calls with n ≥ 1 produce the correct prime. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset */

    seq_t count    = 0;
    seq_t candidate = 3;                  /* start with the first odd prime > 2 */

    while (1) {
        if (is_prime_big(candidate)) {
            if (has_prime_zero_run(candidate)) {
                ++count;
                if (count == n) return candidate;
            }
        }
        candidate += 2;                   /* test only odd numbers */
    }
}
