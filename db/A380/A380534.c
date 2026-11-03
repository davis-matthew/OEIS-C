/*
 * A380534  a(n) = 1 if the least significant nonzero digit in primorial base
 *          representation of n (A049345) is greater than 1, otherwise 0.
 * https://oeis.org/A380534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if the least significant nonzero digit in primorial
 *            base representation of n is > 1, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9575 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static uint64_t primorial_prime(int idx)
{
    static uint64_t primes[128];
    static int primes_cnt = 0;
    static uint64_t next_candidate = 2;
    while (primes_cnt - 1 < idx) {
        uint64_t p = next_candidate;
        while (true) {
            bool is_prime = true;
            for (int i = 0; i < primes_cnt; ++i) {
                uint64_t q = primes[i];
                if (p % q == 0) { is_prime = false; break; }
                if (q * q > p) break;
            }
            if (is_prime) break;
            ++p;
        }
        primes[primes_cnt++] = p;
        next_candidate = p + 1;
    }
    return primes[idx];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset guard; not used for OEIS */
    idx_t r = n;
    int k = 0;
    uint64_t digit = 0;
    while (r > 0) {
        uint64_t p = primorial_prime(k);
        digit = r % p;
        if (digit != 0) break;
        r /= p;
        ++k;
    }
    return (seq_t)(digit > 1);
}
