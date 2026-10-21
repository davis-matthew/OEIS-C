/*
 * A289551  Unsigned byte values of the infamous "09 F9" HD-DVD encryption
 *          key.
 * https://oeis.org/A289551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = key[n mod 16] (periodic repetition of the 16‑byte HD‑DVD
 *            key)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t key[16] = {
    0x09, 0xF9, 0x11, 0x02,
    0x9D, 0x74, 0xE3, 0x5B,
    0xD8, 0x41, 0x56, 0xC5,
    0x63, 0x56, 0x88, 0xC0
};

seq_t a(idx_t n)
{
    return key[(size_t)(n % 16)];
}
