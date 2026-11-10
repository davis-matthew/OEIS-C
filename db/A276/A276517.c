/*
 * A276517  Indices k such that A276516(k) = 0.
 * https://oeis.org/A276517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k >= 1 with [x^k] Product_{j>=1} (1-x^(j^2)) =
 *            0 (A276516(k)=0); computed by DP over squares, then a scan
 *            (sequence conjectured finite, last term 7169)
 * Range:     u8 n <= 82, u16 n <= 173, u32 n <= 173, u64 n <= 173,
 *            u128 n <= 173, big any n
 * Validated: standard, pass, 78 terms through n = 173 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A276517_LIMIT 10000

static int64_t A276517_buf[A276517_LIMIT + 1];
static unsigned A276517_zeros[A276517_LIMIT + 1];
static size_t A276517_count = 0;
static int A276517_ready = 0;

static void A276517_init(void)
{
    size_t i, k, s;
    if (A276517_ready) return;
    for (i = 0; i <= A276517_LIMIT; ++i) A276517_buf[i] = 0;
    A276517_buf[0] = 1;
    for (k = 1; k * k <= A276517_LIMIT; ++k) {
        s = k * k;
        for (i = A276517_LIMIT; i >= s; --i)
            A276517_buf[i] -= A276517_buf[i - s];
    }
    for (i = 1; i <= A276517_LIMIT; ++i)
        if (A276517_buf[i] == 0) A276517_zeros[A276517_count++] = (unsigned)i;
    A276517_ready = 1;
}

seq_t a(idx_t n)
{
    A276517_init();
    if (n == 0 || n > (idx_t)A276517_count) return (seq_t)0;
    return (seq_t)A276517_zeros[(size_t)(n - 1)];
}
