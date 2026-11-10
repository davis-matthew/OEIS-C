/*
 * A293843  Split the infinite binary word A030302, from left to right, into
 *          the largest possible cubefree chunks; a(n) = length of n-th
 *          cubefree chunk.
 * https://oeis.org/A293843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the n-th chunk when the binary Champernowne
 *            word A030302 is split greedily, left to right, into maximal
 *            cubefree factors (computed by direct simulation)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9504 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A293843_bit(uint64_t p)
{
    /* p is 1-based position in A030302 */
    uint64_t rem = p - 1;
    unsigned len = 1;
    while (1) {
        uint64_t bits = ((uint64_t)1 << (len - 1)) * len;
        if (rem < bits) break;
        rem -= bits;
        len++;
    }
    uint64_t idx = rem / len;
    unsigned off = (unsigned)(rem % len);
    uint64_t number = ((uint64_t)1 << (len - 1)) + idx;
    return (number >> (len - 1 - off)) & 1;
}

static unsigned *A293843_lens = 0;
static uint64_t A293843_count = 0, A293843_cap = 0;
static uint64_t A293843_next = 1;
static unsigned char *A293843_buf = 0;
static uint64_t A293843_bcap = 0;

static void A293843_chunk(void)
{
    uint64_t len = 0;
    while (1) {
        unsigned char c = (unsigned char)A293843_bit(A293843_next + len);
        if (len + 1 > A293843_bcap) {
            A293843_bcap = A293843_bcap ? A293843_bcap * 2 : 256;
            A293843_buf = (unsigned char *)realloc(A293843_buf, (size_t)A293843_bcap);
        }
        A293843_buf[len] = c;
        uint64_t L = len + 1;
        int cube = 0;
        for (uint64_t p = 1; 3 * p <= L && !cube; p++) {
            uint64_t ok = 1;
            for (uint64_t i = L - 2 * p; i < L; i++) {
                if (A293843_buf[i] != A293843_buf[i - p]) { ok = 0; break; }
            }
            if (ok) cube = 1;
        }
        if (cube) break;
        len = L;
    }
    if (A293843_count >= A293843_cap) {
        A293843_cap = A293843_cap ? A293843_cap * 2 : 1024;
        A293843_lens = (unsigned *)realloc(A293843_lens, (size_t)(A293843_cap * sizeof(unsigned)));
    }
    A293843_lens[A293843_count++] = (unsigned)len;
    A293843_next += len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A293843_count < (uint64_t)n) A293843_chunk();
    return (seq_t)A293843_lens[n - 1];
}
