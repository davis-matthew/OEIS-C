/*
 * A331580  Smallest number whose unsorted prime signature is the reversed
 *          unsorted prime signature of n.
 * https://oeis.org/A331580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the smallest integer whose multiset of
   prime exponents is the reverse of the multiset of prime exponents
   of the argument n.  For n = 1 it returns 1. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static const uint64_t FIRST_PRIMES[16] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL, 53ULL
};

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    uint64_t tmp = (uint64_t)n;
    uint8_t exps[64];           /* exponents of the prime factors of n */
    size_t k = 0;               /* number of distinct primes */

    /* Trial division to factor n, collecting exponents in ascending
       prime order. */
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            uint8_t cnt = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++cnt;
            }
            exps[k++] = cnt;
        }
    }
    if (tmp > 1) {              /* remaining prime factor */
        exps[k++] = 1;
    }

    /* Compute the minimal number with the reversed exponent list:
       assign the i‑th exponent (from the end of exps) to the i‑th prime. */
    seq_t result = 1;
    for (size_t i = 0; i < k; ++i) {
        uint8_t exp = exps[k - 1 - i];          /* reverse order */
        seq_t pwr = big_pow((seq_t)FIRST_PRIMES[i], (seq_t)exp);
        result = seq_mul(result, pwr);
    }
    return result;
}
