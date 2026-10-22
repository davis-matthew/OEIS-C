/*
 * A355593  a(n) is the number of alternating integers that divide n.
 * https://oeis.org/A355593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_alternating_uint64, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of alternating divisors of n */
/* An integer is "alternating" if its decimal digits alternate in parity
 * (even ↔ odd). Single digit numbers are always alternating. The
 * implementation factors n using the helper function
 * smallest_prime_factor and enumerates all its positive divisors,
 * counting only those that are alternating. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/is_alternating_uint64.h"

/* Recursively generate all divisors from the prime factorisation
 * stored in pf[]/pe[] and increment *cnt for each alternating divisor. */
static void rec_div(idx_t idx, size_t pf_cnt,
                    uint64_t cur, const uint64_t *pf,
                    const uint64_t *pe, seq_t *cnt)
{
    if (idx == pf_cnt) {
        if (is_alternating_uint64(cur)) ++(*cnt);
        return;
    }
    uint64_t prod = 1;
    for (uint64_t e = 0; e <= pe[idx]; ++e) {
        rec_div(idx + 1, pf_cnt, cur * prod, pf, pe, cnt);
        prod *= pf[idx];
    }
}

seq_t a(idx_t n)
{
#if 0
    /* n is assumed to be ≥ 1 (index offset 1).  No special
       handling for n == 0 is required. */
#endif
    uint64_t pf[64];
    uint64_t pe[64];
    size_t pf_cnt = 0;
    uint64_t temp = n;

    /* Factorise n by repeated division using smallest_prime_factor. */
    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        uint64_t e = 0;
        do {
            temp /= p;
            ++e;
        } while (temp % p == 0);
        pf[pf_cnt] = p;
        pe[pf_cnt] = e;
        ++pf_cnt;
    }

    seq_t count = 0;
    rec_div(0, pf_cnt, 1u, pf, pe, &count);
    return count;
}
