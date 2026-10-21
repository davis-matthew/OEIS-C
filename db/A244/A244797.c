/*
 * A244797  Number of moduli m such that (prime(n) mod m) = 2.
 * https://oeis.org/A244797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (prime(n)-2) minus 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Count of divisors of a positive integer x (x ≥ 1). */
static seq_t divisor_count(uint64_t x) {
    if (x <= 1) return 1;          /* τ(1) = 1 */
    seq_t cnt = 1;
    uint64_t tmp = x;
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            int exp = 0;
            while (tmp % d == 0) {
                tmp /= d;
                exp++;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        cnt *= (seq_t)2;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of offset */
    uint32_t prime = get_prime(n);
    if (prime == 2) return 0;        /* p-2 = 0 yields no valid moduli */
    uint64_t m = (uint64_t)prime - 2ULL;  /* p-2 > 0 and odd for p ≥ 3 */
    seq_t dcnt = divisor_count(m);
    return dcnt - 1;                /* subtract divisor 1 */
}
