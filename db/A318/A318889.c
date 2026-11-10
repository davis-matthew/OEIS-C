/*
 * A318889  a(n) = A001065(n) - A001065(A252463(n)).
 * https://oeis.org/A318889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001065(n) - A001065(A252463(n)), where A001065 =
 *            sigma(n)-n, A252463(1)=1, A252463(2n)=n,
 *            A252463(2n+1)=A064989(2n+1)
 * Range:     u8 n <= 209, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9808 (2026-11-08)
 * Helpers:   ipow_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime.h"

static seq_t A001065_a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) {
                sum += (seq_t)i;
            }
            idx_t complement = n / i;
            if (complement < n && complement != i) {
                sum += (seq_t)complement;
            }
        }
    }
    return sum;
}

static inline idx_t A252463_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

static seq_t A252463_a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    if ((n & 1u) == 0u)
    {
        return (seq_t)(n >> 1);
    }

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    uint64_t p = 3u;
    while (p <= m / p)
    {
        if (m % p == 0u)
        {
            uint64_t exp = 0u;
            while (m % p == 0u)
            {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)A252463_prev_prime((idx_t)p);
            seq_t term = ipow_idx(base, (idx_t)exp);
            result = result * term;
        }
        p += 2u;
    }

    if (m > 1u)
    {
        seq_t base = (seq_t)A252463_prev_prime((idx_t)m);
        result = result * base;
    }

    return result;
}

seq_t a(idx_t n) {
    seq_t s1 = A001065_a(n);
    seq_t m = A252463_a(n);
    /* m fits in idx_t for the indices considered (m <= n) */
    seq_t s2 = A001065_a((idx_t)m);
    return s1 - s2;
}
