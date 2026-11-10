/*
 * A328766  Number of nonleading zeros in primorial base expansion of
 *          A276086(n).
 * https://oeis.org/A328766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328620(A276086(n)): write n in primorial base, set m =
 *            prod prime(k+1)^d_k, then count the zeros in the primorial base
 *            expansion of m, excluding leading zeros
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 1569, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 122 terms through n = 9693 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A328766_pr[64];
static int A328766_init = 0;

static void A328766_setup(void)
{
    if (A328766_init) return;
    int cnt = 0;
    uint32_t c = 2;
    while (cnt < 64) {
        bool ok = true;
        for (int i = 0; i < cnt; ++i) {
            if (A328766_pr[i] * A328766_pr[i] > c) break;
            if (c % A328766_pr[i] == 0) { ok = false; break; }
        }
        if (ok) A328766_pr[cnt++] = c;
        c++;
    }
    A328766_init = 1;
}

seq_t a(idx_t n)
{
    A328766_setup();
    seq_t m = 1;
    idx_t r = n;
    int k = 0;
    while (r > 0) {
        uint32_t p = A328766_pr[k];
        idx_t d = r % p;
        r /= p;
        for (idx_t j = 0; j < d; ++j) m = m * (seq_t)p;
        ++k;
    }

    /* primorial base digits of m, low to high */
    seq_t zeros = 0;
    seq_t pending = 0; /* zeros seen since last nonzero digit */
    int i = 0;
    while (m > 0 && i < 64) {
        uint32_t p = A328766_pr[i];
        seq_t d = m % (seq_t)p;
        m = m / (seq_t)p;
        if (d == 0) {
            pending++;
        } else {
            zeros += pending;
            pending = 0;
        }
        ++i;
    }
    return zeros;
}
