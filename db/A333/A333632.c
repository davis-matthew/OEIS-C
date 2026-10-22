/*
 * A333632  Rotational period of the k-th composition in standard order; a(0)
 *          = 0.
 * https://oeis.org/A333632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = A000120(n)/A138904(n)  
 * (Computed by generating the k‑th composition in standard order via the binary
 * representation method and finding its minimal rotational period.)
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Empty composition for n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Collect the 1‑bit positions of n (LSB = position 1). */
    idx_t pos[65];
    size_t poscnt = 0;
    idx_t temp = n;
    idx_t index = 1;
    while (temp) {
        if (temp & (idx_t)1)
            pos[poscnt++] = index;
        temp >>= 1;
        index++;
    }

    /* Length of the composition equals the number of set bits. */
    size_t len = poscnt;
    if (len == 0)          /* should not happen for n > 0, but safe */
        return (seq_t)0;

    /* Compute forward differences of the positions. */
    seq_t diff[65];
    diff[0] = (seq_t)pos[0];
    for (size_t i = 1; i < len; ++i)
        diff[i] = (seq_t)(pos[i] - pos[i - 1]);

    /* Reverse the differences to obtain the parts of the composition. */
    seq_t parts[65];
    for (size_t i = 0; i < len; ++i)
        parts[i] = diff[len - 1 - i];

    /* Find the minimal rotation period. */
    seq_t period = (seq_t)len;
    for (size_t d = 1; d <= len; ++d) {
        if (len % d != 0)
            continue;
        bool ok = true;
        for (size_t i = 0; i < len; ++i) {
            size_t j = (i + d) % len;
            if (parts[i] != parts[j]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            period = (seq_t)d;
            break;
        }
    }

    return period;
}
