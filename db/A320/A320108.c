/*
 * A320108  a(n) = Product_{d|n, d>1} prime(1+(d mod 8)).
 * https://oeis.org/A320108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(1+(d mod 8))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 59, u64 n <= 419,
 *            u128 n <= 3779, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Mapping of d mod 8 to the (1+(d mod 8))-th prime:
   d%8 :   0   1   2   3   4   5   6   7
   prime:  2   3   5   7  11  13  17  19 */
static const seq_t PRIME_FOR_MOD[8] = {2,3,5,7,11,13,17,19};

/* Recursive enumeration of all divisors of n (including 1) */
static void enumerate_divisors(const uint64_t *pr, const uint64_t *ex,
                               size_t cnt, size_t idx, uint64_t cur,
                               seq_t *prod)
{
    if (idx == cnt) {
        /* Include this divisor */
        uint8_t r = (uint8_t)(cur & 7);   /* cur % 8 */
        *prod *= PRIME_FOR_MOD[r];
        return;
    }
    uint64_t p = pr[idx];
    uint64_t cur_val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        enumerate_divisors(pr, ex, cnt, idx + 1, cur_val, prod);
        cur_val *= p;
    }
}

seq_t a(idx_t n)
{
    /* Factor n */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Product over all divisors (including 1) */
    seq_t prod = 1;
    enumerate_divisors(pr, ex, cnt, 0, 1ull, &prod);

    /* Remove the contribution of divisor 1:  prime(1+(1%8)) = 3 */
    prod /= (seq_t)3;
    return prod;
}
