/*
 * A390841  2-valuation of Product_{k=1..n} (prime(k) + 1) / (prime(k+1) -
 *          prime(k)).
 * https://oeis.org/A390841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} v2(prime(k)+1) - v2(prime(k+1)-prime(k))
 * Range:     u8 n <= 554, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, v2_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/v2_uint32.h"

seq_t a(idx_t n)
{
    long long acc = 0;          /* intermediate sum may be negative temporarily */
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t p     = get_prime(k);
        uint32_t p_next = get_prime(k + 1);
        uint32_t denom = p_next - p;
        int v_num  = v2_uint32(p + 1U);
        int v_den  = v2_uint32(denom);
        acc += (long long)v_num - (long long)v_den;
    }
    return (seq_t)acc;
}
