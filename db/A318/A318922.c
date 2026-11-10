/*
 * A318922  Apply Lenormand's transformation k -> A318921(k) to the Fibonacci
 *          numbers.
 * https://oeis.org/A318922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A318921(A000045(n)); F(n) is computed exactly in a
 *            multi-limb binary array, then one bit is deleted from each run
 *            of its binary expansion and the remaining bits are concatenated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 88, u64 n <= 167,
 *            u128 n <= 340, big any n
 * Validated: standard, pass, 74 terms through n = 8835 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Compute F(n) into a little-endian array of 32-bit limbs; returns limb count. */
static size_t A318922_fib_limbs(idx_t n, uint32_t **out)
{
    size_t cap = (size_t)(n * 7 / 320) + 4;   /* log2(phi) < 0.7 bits per step */
    uint32_t *x = (uint32_t *)calloc(cap, sizeof(uint32_t)); /* F(i)   */
    uint32_t *y = (uint32_t *)calloc(cap, sizeof(uint32_t)); /* F(i+1) */
    size_t len = 1;
    y[0] = 1;
    for (idx_t i = 0; i < n; i++) {
        /* x = x + y; then swap so (x,y) = (F(i+1), F(i+2)) */
        uint64_t carry = 0;
        for (size_t k = 0; k < len; k++) {
            uint64_t s = (uint64_t)x[k] + y[k] + carry;
            x[k] = (uint32_t)s;
            carry = s >> 32;
        }
        if (carry) {
            x[len] = (uint32_t)carry;
            len++;
        }
        uint32_t *t = x; x = y; y = t;
    }
    free(y);
    *out = x;
    while (len > 0 && x[len - 1] == 0) len--;
    return len;
}

seq_t a(idx_t n)
{
    uint32_t *f;
    size_t len = A318922_fib_limbs(n, &f);
    if (len == 0) { free(f); return 0; }

    uint32_t top = f[len - 1];
    unsigned tb = 0;
    while (top) { tb++; top >>= 1; }
    size_t L = (len - 1) * 32 + tb;

    seq_t res = 0;
    int run_bit = 1;
    size_t run_len = 0;
    for (size_t j = L; j-- > 0; ) {
        int bit = (int)((f[j >> 5] >> (j & 31)) & 1u);
        if (bit == run_bit) {
            ++run_len;
        } else {
            for (size_t k = 1; k < run_len; ++k)
                res = (res << 1) | (seq_t)run_bit;
            run_bit = bit;
            run_len = 1;
        }
    }
    for (size_t k = 1; k < run_len; ++k)
        res = (res << 1) | (seq_t)run_bit;

    free(f);
    return res;
}
