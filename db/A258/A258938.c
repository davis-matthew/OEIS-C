/*
 * A258938  Index of the Lucas numbers in A258740.
 * https://oeis.org/A258938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k-index of n-th Lucas number L(k), k>=2, with
 *            digit_sum(L(k)) > k; found by search with L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2) in a base-10^9 big integer
 * Range:     u8 n <= 73, u16 n <= 228, u32 n <= 228, u64 n <= 228,
 *            u128 n <= 228, big any n
 * Validated: standard, pass, 76 terms through n = 227 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A258938_LIMBS 256
#define A258938_MAXK 8000
#define A258938_BASE 1000000000U

static uint16_t A258938_terms[1024];
static size_t A258938_count = 0;
static bool A258938_done = false;

static void A258938_build(void)
{
    static uint32_t buf[2][A258938_LIMBS];
    int xi = 0, yi = 1, i, k;
    int len = 1;
    for (i = 0; i < A258938_LIMBS; i++) { buf[0][i] = 0; buf[1][i] = 0; }
    buf[0][0] = 2; /* L(0) */
    buf[1][0] = 1; /* L(1) */
    for (k = 2; k <= A258938_MAXK; k++) {
        uint32_t *x = buf[xi];
        uint32_t *y = buf[yi];
        uint32_t carry = 0;
        uint64_t ds = 0;
        for (i = 0; i < len || carry; i++) {
            uint32_t s = x[i] + y[i] + carry;
            if (s >= A258938_BASE) { s -= A258938_BASE; carry = 1; }
            else carry = 0;
            x[i] = s;
            if (i >= len) len = i + 1;
        }
        for (i = 0; i < len; i++) {
            uint32_t v = x[i];
            while (v) { ds += v % 10; v /= 10; }
        }
        if (ds > (uint64_t)k && A258938_count < 1024)
            A258938_terms[A258938_count++] = (uint16_t)k;
        /* x now holds L(k); y holds L(k-1); swap so y = newest */
        xi ^= 1;
        yi ^= 1;
    }
}

seq_t a(idx_t n)
{
    if (!A258938_done) {
        A258938_build();
        A258938_done = true;
    }
    if (n >= 1 && (size_t)n <= A258938_count)
        return (seq_t)A258938_terms[(size_t)n - 1];
    return 0;
}
