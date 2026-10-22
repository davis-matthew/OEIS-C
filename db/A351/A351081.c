/*
 * A351081  a(n) = Product_{d|n} A019565(A289813(d)); a product obtained from
 *          the 1-digits present in ternary expansions of the divisors of n.
 * https://oeis.org/A351081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{d|n} ∏_{k: ternary digit k of d = 1} prime_k where
 *            prime_k is the (k+1)-th prime (starting with 2 for k=0)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint32_t _primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,179,181,191,193,
    197,199,211,223,227,229,233,239,241,251,257,263,269,
    271,277,281,283,293,307,311
};
static const size_t _prime_count = sizeof(_primes)/sizeof(_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 for safety */

    /* Find maximum ternary digit position needed (i.e., for n itself) */
    idx_t tmp = n;
    size_t max_pos = 0;
    while (tmp > 0) {
        tmp /= 3;
        ++max_pos;
    }
    if (max_pos == 0) max_pos = 1;

    /* (Optional) ensure we have enough primes; not needed for typical n */
    if (max_pos > _prime_count) {
        /* In practice n <= 10^12 ⇒ max_pos < 40, so this branch never runs.  */
    }

    seq_t result = 1;

    /* Enumerate all divisors of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* Process divisor i */
            idx_t d = i;
            seq_t g = 1;
            size_t pos = 0;
            idx_t x = d;
            while (x > 0) {
                uint8_t digit = x % 3;
                if (digit == 1) {
                    g *= (seq_t)_primes[pos];
                }
                x /= 3;
                ++pos;
            }
            result *= g;

            /* Process paired divisor j = n / i, skip if same as i */
            idx_t j = n / i;
            if (j != i) {
                d = j;
                g = 1;
                pos = 0;
                x = d;
                while (x > 0) {
                    uint8_t digit = x % 3;
                    if (digit == 1) {
                        g *= (seq_t)_primes[pos];
                    }
                    x /= 3;
                    ++pos;
                }
                result *= g;
            }
        }
    }
    return result;
}
