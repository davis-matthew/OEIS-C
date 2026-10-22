/*
 * A387412  The length of the maximal common prefix of the binary expansions
 *          of n and A003961(n), where A003961 is fully multiplicative with
 *          a(p) = nextprime(p).
 * https://oeis.org/A387412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of maximal common prefix of the binary expansions
 *            of n and A003961(n), where A003961 is fully multiplicative with
 *            a(p) = nextprime(p)
 * Range:     u8 n <= 9829, u16 n <= 9829, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   _smallest_prime_factor, floor_log2_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t) */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t) */
#include "../utils/floor_log2_uint64.h"  /* unsigned floor_log2_uint64(idx_t) */

seq_t a(idx_t n)
{
    /* Compute the value of A003961(n) by its multiplicative definition.
       The intermediate value is kept in a 64‑bit unsigned integer because
       for n <= 10000 the product always fits in 64 bits.  This is true in
       all expected use cases for the sequence. */
    uint64_t prod = 1;
    uint64_t m    = (uint64_t)n;

    while (m > 1) {
        uint64_t p   = _smallest_prime_factor(m);
        uint64_t exp = 0;

        while (m % p == 0) {
            m /= p;
            exp++;
        }

        uint64_t np   = next_prime(p);          /* next prime after p */
        uint64_t mult = 1;

        for (uint64_t i = 0; i < exp; ++i)
            mult *= np;

        prod *= mult;
    }

    /* Length (in bits) of the binary representations of n and prod. */
    unsigned len_n = floor_log2_uint64(n) + 1;
    unsigned len_p = floor_log2_uint64(prod) + 1;
    unsigned minlen = (len_n < len_p) ? len_n : len_p;

    /* Count matching most significant bits. */
    unsigned prefix = 0;
    for (unsigned i = 0; i < minlen; ++i) {
        unsigned bit_n = (unsigned)((n >> (len_n - 1 - i)) & 1ull);
        unsigned bit_p = (unsigned)((prod >> (len_p - 1 - i)) & 1ull);
        if (bit_n == bit_p)
            ++prefix;
        else
            break;
    }

    return (seq_t)prefix;
}
