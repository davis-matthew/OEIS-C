/*
 * A381581  Numbers divisible by the sum of the digits in their Chung-Graham
 *          representation (A381579).
 * https://oeis.org/A381581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n-th k >= 1 such that A291711(k) divides k, where
 *            A291711(k) = minimal number of coins from {F(2j)} (= digit sum
 *            of the Chung-Graham representation); found by a direct
 *            increasing search
 * Range:     u8 n <= 68, u16 n <= 8379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9102 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A381581_coins_ready = 0;
static uint64_t A381581_coins[64];
static int A381581_ncoins = 0;

static void A381581_init(void)
{
    if (A381581_coins_ready) return;
    uint64_t f0 = 0, f1 = 1;
    int idx = 1;
    A381581_ncoins = 0;
    while (1) {
        if (f0 > UINT64_MAX - f1) break;
        uint64_t f2 = f0 + f1;
        f0 = f1;
        f1 = f2;
        ++idx;
        if ((idx & 1) == 0) {
            A381581_coins[A381581_ncoins++] = f1;
        }
        if (idx >= 92) break;
    }
    A381581_coins_ready = 1;
}

static uint64_t A381581_count(uint64_t k)
{
    uint64_t rem = k, cnt = 0;
    int i = A381581_ncoins;
    while (i-- > 0) {
        uint64_t c = A381581_coins[i];
        if (c <= rem) {
            uint64_t q = rem / c;
            cnt += q;
            rem -= q * c;
            if (rem == 0) break;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    A381581_init();
    uint64_t found = 0;
    uint64_t k = 0;
    while (found < (uint64_t)n) {
        ++k;
        uint64_t s = A381581_count(k);
        if (s != 0 && k % s == 0) ++found;
    }
    return (seq_t)k;
}
