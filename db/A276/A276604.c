/*
 * A276604  After a(0)=0, the first differences of A276623.
 * https://oeis.org/A276604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n >= 1, a(n) = A053735(A276623(n)) = A276623(n) -
 *            A276623(n-1). A276623 (the infinite trunk) passes through every
 *            3^k - 1. The code walks down from 3^k - 1 with x -> x -
 *            A053735(x) until it reaches 0, then reverses that path to get
 *            A276623(0..m). It uses a large enough k
 * Range:     u8 n <= 6250, u16 n <= 6250, u32 n <= 6250, u64 n <= 6250,
 *            u128 n <= 6250, big any n
 * Validated: standard, pass, 130 terms through n = 6178 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t A276604_s3(uint64_t x)
{
    uint64_t s = 0;
    for (; x != 0; x /= 3)
        s += x % 3;
    return s;
}

static uint64_t *A276604_vals = 0;
static idx_t A276604_cnt = 0; /* number of stored terms A276623(0..cnt-1) */
static int A276604_k = 0;

static void A276604_build(idx_t need)
{
    while (A276604_cnt <= need) {
        int k = A276604_k + 1;
        uint64_t top = 1;
        for (int i = 0; i < k; i++) top *= 3;
        top -= 1;
        /* count chain length */
        idx_t len = 1;
        for (uint64_t x = top; x != 0; x -= A276604_s3(x)) len++;
        uint64_t *v = (uint64_t *)malloc((size_t)len * sizeof(uint64_t));
        idx_t i = len;
        uint64_t x = top;
        while (1) {
            v[--i] = x;
            if (x == 0) break;
            x -= A276604_s3(x);
        }
        if (A276604_vals) free(A276604_vals);
        A276604_vals = v;
        A276604_cnt = len;
        A276604_k = k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A276604_build(n);
    return (seq_t)(A276604_vals[n] - A276604_vals[n - 1]);
}
