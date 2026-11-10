/*
 * A302175  a(n) = [2^A006666(n)/3^A006667(n)], where [x] = floor(x).
 * https://oeis.org/A302175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^A006666(n) / 3^A006667(n)); exponents counted
 *            along the Collatz trajectory, quotient by exact multi-limb
 *            division
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t cur = (uint64_t)n;
    uint64_t h = 0, t = 0;
    while (cur != 1) {
        if (cur % 2 == 0) {
            cur /= 2;
            h++;
        } else {
            cur = cur * 3 + 1;
            t++;
        }
    }

    /* Build 2^h in 16-bit limbs (little endian). */
    size_t L = (size_t)(h / 16) + 1;
    uint32_t *limb = (uint32_t *)malloc(L * sizeof(uint32_t));
    for (size_t i = 0; i < L; i++) limb[i] = 0;
    limb[h / 16] = (uint32_t)1 << (h % 16);

    /* Divide by 3^t in chunks of at most 3^20. */
    uint64_t rem_t = t;
    while (rem_t > 0) {
        uint64_t k = rem_t > 20 ? 20 : rem_t;
        uint64_t d = 1;
        for (uint64_t i = 0; i < k; i++) d *= 3;
        uint64_t r = 0;
        for (size_t i = L; i-- > 0;) {
            uint64_t v = (r << 16) | limb[i];
            limb[i] = (uint32_t)(v / d);
            r = v % d;
        }
        rem_t -= k;
    }

    /* Assemble result into seq_t. */
    size_t top = L;
    while (top > 0 && limb[top - 1] == 0) top--;
    seq_t res = 0;
    for (size_t i = top; i-- > 0;) {
        res <<= 8;
        res <<= 8;
        res += (seq_t)limb[i];
    }
    free(limb);
    return res;
}
