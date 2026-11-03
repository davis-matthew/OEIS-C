/*
 * A317947  Concatenate n and n-1 in binary.
 * https://oeis.org/A317947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(n) * 10^(A000523(n-1)) + A007088(n-1) where
 *            A007088(k) is the binary representation of k written as a
 *            decimal integer composed of digits 0 and 1. A000523(k) is the
 *            number of bits in the binary representation of k, with
 *            A000523(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 31, u64 n <= 1023,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 45 terms through n = 9667 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t bin_as_decimal(idx_t x)
{
    /* Convert x to its binary representation and write it as a decimal
       integer whose digits are the binary bits. */
    if (x == 0)
        return (seq_t)0;

    seq_t res = 0;
    /* Find most significant bit */
    idx_t t = x;
    unsigned msb = 0;
    while (t >> 1) { t >>= 1; ++msb; }

    for (int i = msb; i >= 0; --i)
    {
        seq_t bit = (seq_t)((x >> i) & 1ULL);
        res = res * (seq_t)10 + bit;
    }
    return res;
}

static unsigned bitlen(idx_t x)
{
    /* Number of bits in x; A000523(0)=1. */
    unsigned len = 1;
    while (x > 1)
    {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* The algorithm works for all n >= 1 including n==1 (handled
       automatically by the bitlen routine). */
    idx_t m = n - 1;
    unsigned len_m = bitlen(m);          /* A000523(m) */
    seq_t pow_factor = pow10((idx_t)len_m); /* 10^len_m */
    seq_t bin_n = bin_as_decimal(n);
    seq_t bin_m = bin_as_decimal(m);
    return bin_n * pow_factor + bin_m;
}
