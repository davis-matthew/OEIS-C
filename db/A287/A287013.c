/*
 * A287013  Decimal expansion of Sum_{n>=0} 1/A000178(n).
 * https://oeis.org/A287013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit of Sum_{k>=0} 1/A000178(k) = 2.586834530936...
 *            (a(1) is the integer part 2); computed by summing
 *            1/(1!*2!*...*k!) in fixed-point base 10^9 arithmetic with guard
 *            limbs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9149 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A287013_sum = 0;
static idx_t A287013_cap = 0;   /* fractional digits available */

static void A287013_build(idx_t D)
{
    size_t L = (size_t)((D + 8) / 9) + 3;
    uint32_t *t = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    size_t j;
    unsigned k, i;
    t[0] = 1;   /* term for k = 0: 1/A000178(0) = 1 */
    s[0] = 1;
    for (k = 1;; k++) {
        int nz = 0;
        uint32_t carry = 0;
        /* t <- t / k!  (divide by 2..k; dividing by 1 is a no-op) */
        for (i = 2; i <= k; i++) {
            uint64_t rem = 0;
            for (j = 0; j <= L; j++) {
                uint64_t cur = rem * 1000000000ULL + t[j];
                t[j] = (uint32_t)(cur / i);
                rem = cur % i;
            }
        }
        for (j = 0; j <= L; j++) if (t[j]) { nz = 1; break; }
        if (!nz) break;
        for (j = L + 1; j-- > 0;) {
            uint64_t v = (uint64_t)s[j] + t[j] + carry;
            if (j > 0) {
                carry = (uint32_t)(v / 1000000000ULL);
                s[j] = (uint32_t)(v % 1000000000ULL);
            } else {
                s[j] = (uint32_t)v;
                carry = 0;
            }
        }
    }
    free(t);
    free(A287013_sum);
    A287013_sum = s;
    A287013_cap = (idx_t)((L - 3) * 9);
}

seq_t a(idx_t n)
{
    idx_t m, p, r;
    uint32_t v;
    int e;
    if (!A287013_sum || n > A287013_cap) {
        idx_t D = n * 2 + 20;
        if (D < 200) D = 200;
        A287013_build(D);
    }
    if (n <= 1) return (seq_t)(A287013_sum[0] % 10);   /* integer part: 2 */
    m = n - 1;            /* fractional digit index, m >= 1 */
    p = (m - 1) / 9;
    r = (m - 1) % 9;
    v = A287013_sum[1 + p];
    for (e = 0; e < (int)(8 - r); e++) v /= 10;
    return (seq_t)(v % 10);
}
