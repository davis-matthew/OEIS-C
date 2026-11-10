/*
 * A392874  a(n) = A005940(1+n) mod A117366(A005940(1+n)), where A117366 gives
 *          the smallest prime greater than the largest prime dividing n, and
 *          A005940 is Doudna-sequence.
 * https://oeis.org/A392874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A005940(n+1) mod A117366(A005940(n+1)); with Doudna value
 *            d = product of prime(i) over the 1-bits of n (i = 1 + zeros
 *            below that bit), the modulus is prime(k+1), k = largest index
 *            used
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 117 terms through n = 9970 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t a392874_prime(int k)
{
    uint64_t c = 1;
    int cnt = 0;
    while (cnt < k) {
        c++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t m = n;
    int i = 1, maxi = 0;
    while (m > 0) {
        if (m & 1) {
            maxi = i;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    if (maxi == 0)
        return (seq_t)1; /* d = 1, modulus 2 */

    uint64_t p = a392874_prime(maxi + 1);
    seq_t result = 1;
    m = n;
    i = 1;
    while (m > 0) {
        if (m & 1) {
            result = (result * (seq_t)a392874_prime(i)) % (seq_t)p;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}
