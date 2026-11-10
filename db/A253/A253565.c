/*
 * A253565  Permutation of natural numbers: a(0) = 1, a(1) = 2; after which,
 *          a(2n) = A253550(a(n)), a(2n+1) = A253560(a(n)).
 * https://oeis.org/A253565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(0) = 1, a(1) = 2; a(2n) = A253550(a(n)), a(2n+1) =
 *            A253560(a(n)). Computed by tracking the prime exponent vector of
 *            a(n) while reading the binary digits of n from the top: bit 0
 *            moves one copy of the largest prime to the next prime, bit 1
 *            multiplies by the largest prime
 * Range:     u8 n <= 34, u16 n <= 270, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 97 terms through n = 8039 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A253565_MAXP 140

static uint32_t A253565_primes[A253565_MAXP];
static bool A253565_ready = false;

static void A253565_init(void)
{
    int cnt = 0;
    uint32_t c = 2;
    while (cnt < A253565_MAXP) {
        bool ok = true;
        for (int i = 0; i < cnt; ++i) {
            if ((uint32_t)A253565_primes[i] * A253565_primes[i] > c) break;
            if (c % A253565_primes[i] == 0) { ok = false; break; }
        }
        if (ok) A253565_primes[cnt++] = c;
        ++c;
    }
    A253565_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (!A253565_ready) A253565_init();

    int e[A253565_MAXP + 2];
    for (int i = 0; i < A253565_MAXP + 2; ++i) e[i] = 0;

    int top = 63;
    while (!((n >> top) & 1)) --top;

    int g = 0;          /* index (0-based) of largest prime */
    e[0] = 1;           /* a(1) = 2 */

    for (int b = top - 1; b >= 0; --b) {
        if ((n >> b) & 1) {
            e[g]++;
        } else {
            e[g]--;
            e[g + 1]++;
            g++;
        }
    }

    seq_t result = (seq_t)1;
    for (int i = 0; i <= g; ++i) {
        for (int k = 0; k < e[i]; ++k)
            result *= (seq_t)A253565_primes[i];
    }
    return result;
}
