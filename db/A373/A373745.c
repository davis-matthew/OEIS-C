/*
 * A373745  Maximum length of a run of alternating bits in the base-2
 *          representation of prime(n).
 * https://oeis.org/A373745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A374176(prime(n)) + 1 (maximum alternating‑bit run + 1)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* undefined for n = 0, harmless */
    }

    /* obtain the nth prime (fits in 32 bits for n up to 10 000) */
    uint32_t p = get_prime(n);

    /* compute bit length of p */
    int bits = 0;
    uint32_t tmp = p;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }
    if (bits == 0) {
        return (seq_t)0;            /* should not happen for prime >= 2 */
    }

    /* find longest alternating‑bit run in the binary representation of p */
    int max_len = 1, cur_len = 1;
    int prev_bit = (int)((p >> (bits - 1)) & 1);

    for (int i = bits - 2; i >= 0; --i) {
        int bit = (int)((p >> i) & 1);
        if (bit != prev_bit) {
            cur_len++;
        } else {
            cur_len = 1;
        }
        if (cur_len > max_len) {
            max_len = cur_len;
        }
        prev_bit = bit;
    }

    return (seq_t)max_len;
}
