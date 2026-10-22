/*
 * A393916  a(n) is the least positive integer k such that k times the n-th
 *          triangular number is pentagonal.
 * https://oeis.org/A393916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer k such that k * triangular(n)
 *            is a pentagonal number. The triangular number T_n = n(n+1)/2.
 *            Find the smallest k where 1 + 24kT_n is a perfect square s^2
 *            with s ≡ 5 (mod 6). This is equivalent to the Pell‑type equation
 *            s² - 24T_n·k = 1. Here we solve it by a simp...
 * Range:     u8 n <= 30, u16 n <= 447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

seq_t a(idx_t n)
{
    /* triangular number T = n(n+1)/2 */
    unsigned long long t = (unsigned long long)n * ((unsigned long long)n + 1) / 2;
    unsigned long long denom = 2 * t;          /* 2*T */

    for (idx_t m = 1; ; ++m) {
        unsigned __int128 m128     = (unsigned __int128)m;
        unsigned __int128 numerator = 3 * m128 * m128 - m128;   /* 3m² - m */

        /* if numerator is divisible by denom, we found the minimal k */
        if ((uint64_t)(numerator % denom) == 0) {
            unsigned __int128 k128 = numerator / denom;
            return (seq_t)(uint64_t)k128;   /* cast fits when term < 2⁶⁴ */
        }
    }
}
