/*
 * A253585  Numbers whose binary expansion equals the first n digits of the
 *          binary sequence A252488 whose run lengths are given by A001511
 *          (the ruler function).
 * https://oeis.org/A253585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer represented by the first n bits of the binary *
 *            sequence obtained by alternating runs of 1s and 0s whose lengths
 *            are * given by v₂(k)+1 for k = 1,2,3,... (v₂(k) = exponent of
 *            the highest * power of 2 dividing k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the binary prefix bit‑by‑bit. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t value = 0;
    idx_t run  = 1;                         /* current run number      */
    idx_t rem  = (__builtin_ctzll((unsigned long long)run) + 1); /* bits left in run   */
    int curBit = 1;                         /* 1 for odd runs, 0 for even   */

    for (idx_t i = 0; i < n; ++i) {
        /* Append next bit */
        value = (value << 1) | (seq_t)curBit;
        --rem;
        if (rem == 0) {
            /* Move to next run */
            ++run;
            curBit = (run & 1) ? 1 : 0;
            rem    = (__builtin_ctzll((unsigned long long)run) + 1);
        }
    }
    return value;
}
