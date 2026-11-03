/*
 * A317587  a(n) is the smallest number m > n such that Sum_{k=1..n-1} k^(m-1)
 *          == n-1 (mod m).
 * https://oeis.org/A317587
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 70 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A317587: a(n) = smallest m>n with Σ_{k=1}^{n-1} k^(m-1) ≡ n-1 (mod m). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* sequence defined for n ≥ 2 only */

    for (idx_t m = n + 1; ; ++m)
    {
        /* Any prime > n automatically satisfies the condition. */
        if (is_prime(m))
            return (seq_t)m;

        seq_t m_seq = (seq_t)m;
        seq_t sum = 0;
        for (idx_t k = 1; k < n; ++k)
        {
            seq_t term = powmod((seq_t)k, (uint64_t)(m - 1), m_seq);
            sum = (sum + term) % m_seq;
        }
        if (sum == (seq_t)(n - 1))
            return (seq_t)m;
    }
}
