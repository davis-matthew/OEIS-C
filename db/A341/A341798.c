/*
 * A341798  a(n) is the starting position of the first occurrence of the
 *          binary reversal of n in the binary Champernowne word (A030190).
 * https://oeis.org/A341798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by searching for the binary reversal of n * in the
 *            binary Champernowne word (A030190)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It uses a simple on‑demand search: the binary Champernowne word is
 * generated bit by bit.  For each bit the last `len` bits are kept in a
 * circular buffer where `len` is the number of bits of the binary
 * representation of n.  Whenever enough bits have been collected we
 * compare them with the reverse of n's bits.  The smallest start
 * position found is returned.  This works for every non‑negative n and
 * uses only integer arithmetic.  Complexity is O(L * N) where L is the
 * bit length of n, and N is the number of bits examined until the
 * pattern first appears. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* length of binary representation of n (at least 1) */
    int patlen;
    if (n == 0) {
        patlen = 1;
    } else {
        uint64_t tmp = n;
        patlen = 0;
        while (tmp) {
            patlen++;
            tmp >>= 1;
        }
    }

    /* reverse bits of n (stored LSB first) */
    uint8_t pattern[64];
    for (int j = 0; j < patlen; ++j)
        pattern[j] = (uint8_t)((n >> j) & 1U);

    uint8_t buf[64];                /* circular buffer of last `patlen` bits */
    idx_t pos = 0;                  /* global bit position (0‑based) */
    idx_t patlen_t = (idx_t)patlen; /* for modulus with idx_t */

    /* generate the binary Champernowne word and search */
    for (idx_t k = 0;; ++k) {
        uint64_t val = k;
        int bitlen_k;
        if (k == 0) {
            bitlen_k = 1;
        } else {
            uint64_t t = val;
            bitlen_k = 0;
            while (t) {
                bitlen_k++;
                t >>= 1;
            }
        }

        for (int bi = bitlen_k - 1; bi >= 0; --bi) {
            uint8_t bit = (uint8_t)((val >> bi) & 1U);
            buf[pos % patlen] = bit;

            if (pos >= (idx_t)(patlen - 1)) {
                idx_t start = pos - (idx_t)(patlen - 1);
                int ok = 1;
                for (int j = 0; j < patlen; ++j) {
                    if (buf[(start + (idx_t)j) % patlen_t] != pattern[j]) {
                        ok = 0;
                        break;
                    }
                }
                if (ok)
                    return (seq_t)start;
            }
            ++pos;
        }
    }

    /* unreachable but keeps the compiler happy */
    return (seq_t)-1;
}
