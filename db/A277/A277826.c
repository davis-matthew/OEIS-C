/*
 * A277826  a(n) = the least k for which A264977(k) = A264977(n).
 * https://oeis.org/A277826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k such that A264977(k) = A264977(n), where
 *            A264977(0)=0, A264977(1)=1, A264977(2m)=2*A264977(m),
 *            A264977(2m+1)=A264977(m) XOR A264977(m+1); computed by a linear
 *            scan
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9602 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *b = mem_new(seq_t, (size_t)n + 3);
    idx_t i;
    idx_t res = n;
    b[0] = 0;
    b[1] = 1;
    for (i = 2; i <= n; i++) {
        if ((i & 1ULL) == 0)
            b[i] = 2 * b[i >> 1];
        else
            b[i] = b[i >> 1] ^ b[(i >> 1) + 1];
    }
    for (i = 0; i <= n; i++) {
        if (b[i] == b[n]) { res = i; break; }
    }
    mem_free(b);
    return (seq_t)res;
}
