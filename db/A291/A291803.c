/*
 * A291803  Partial sums of A230625.
 * https://oeis.org/A291803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A230625(k), where A230625(k) concatenates
 *            the binary forms of the prime factorization of k (primes, and
 *            exponents >1) and reads the result as a binary number
 * Range:     u8 n <= 17, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9493 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A291803_A230625(uint64_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 0;
    uint64_t temp = n;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            int plen = 0;
            for (uint64_t q = p; q; q >>= 1) plen++;
            result = (result << plen) | (seq_t)p;
            if (e > 1) {
                int elen = 0;
                for (uint64_t r = e; r; r >>= 1) elen++;
                result = (result << elen) | (seq_t)e;
            }
        }
    }
    if (temp > 1) {
        int plen = 0;
        for (uint64_t q = temp; q; q >>= 1) plen++;
        result = (result << plen) | (seq_t)temp;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k)
        sum += A291803_A230625(k);
    return sum;
}
