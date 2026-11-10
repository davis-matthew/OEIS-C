/*
 * A243066  Permutation of natural numbers, the even bisection of A241909
 *          incremented by one and halved; equally, a composition of A241909
 *          and A048673: a(n) = A048673(A241909(n)).
 * https://oeis.org/A243066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = (A241909(2*n)+1)/2, where A241909(m) for m =
 *            p_i1*...*p_ik (index-sorted) is 2^(i1-1) * 3^(i2-i1) * ... *
 *            p_k^(1+ik-i_{k-1})
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 72, u64 n <= 180,
 *            u128 n <= 420, big any n
 * Validated: standard, pass, 70 terms through n = 464 (2026-11-07)
 * Helpers:   factor_u64, ipow_idx, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n * 2u, pr, ex, &cnt);

    idx_t ind[130];
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t pi = (idx_t)prime_pi((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) {
            ind[total++] = pi;
        }
    }

    seq_t result = (seq_t)1;
    for (size_t j = 0; j < total; ++j) {
        uint64_t p = nth_prime((idx_t)(j + 1));
        idx_t exp;
        if (j == 0) {
            exp = ind[0] - 1;
            if (total == 1) exp += 1;
        } else if (j == total - 1) {
            exp = 1 + ind[j] - ind[j - 1];
        } else {
            exp = ind[j] - ind[j - 1];
        }
        result = result * ipow_idx((seq_t)p, exp);
    }
    return (result + 1) / 2;
}
