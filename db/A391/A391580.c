/*
 * A391580  Numbers k for which A391571(k) > k.
 * https://oeis.org/A391580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A391571(k) > k; found by search (k
 *            restricted to A004780, i.e. binary expansion has adjacent 1's)
 * Range:     u8 n <= 16, u16 n <= 729, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9622 (2026-11-10)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"

static seq_t A391571_cdiv(seq_t dividend, seq_t divisor)
{
    seq_t rem = dividend;
    seq_t quo = 0;
    if (divisor == 0)
        return 0;
    int div_deg = (int)bit_length(divisor) - 1;
    while (rem != 0) {
        int rem_deg = (int)bit_length(rem) - 1;
        if (rem_deg < div_deg)
            break;
        int shift = rem_deg - div_deg;
        quo |= ((seq_t)1 << shift);
        rem ^= (divisor << shift);
    }
    return (rem == 0) ? quo : 0;
}

static seq_t A391571_a(seq_t sn)
{
    if (sn == 0)
        return 0;
    for (seq_t k = 3; ; k += 2) {
        seq_t prod = sn * k;
        seq_t q = A391571_cdiv(prod, k);
        if (q != 0)
            return q;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 0;
    while (count < n) {
        num++;
        if ((num & (num << 1)) == 0)
            continue;
        if (A391571_a(num) > num)
            count++;
    }
    return num;
}
