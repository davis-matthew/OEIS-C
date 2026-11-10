/*
 * A345004  Numbers k such that A345001(k)*A048250(k) is equal to
 *          A342001(k)*A344753(k) and A345001(n)/A342001(n) is an integer.
 * https://oeis.org/A345004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k with A345001(k)*A048250(k) = A342001(k)*A344753(k)
 *            and A342001(k) | A345001(k). Computed by a sieve search for k <=
 *            2^25, then the even perfect numbers 2^(p-1)*(2^p-1) beyond that
 *            (they satisfy the condition identically: A345001/A342001 =
 *            2^(p-2))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const int A345004_exps[] = {17, 19, 31, 61, 89, 107, 127};

static uint64_t A345004_small[32];
static int A345004_nsmall = 0;
static int A345004_ready = 0;

static void A345004_init(void)
{
    const uint32_t L = 1u << 25;
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    if (!spf) { A345004_ready = 1; return; }
    for (uint32_t i = 2; (uint64_t)i * i <= L; ++i) {
        if (spf[i] == 0) {
            for (uint32_t j = i * i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (uint32_t n = 2; n <= L; ++n) {
        uint32_t m = n;
        __int128 sigma = 1, psi = 1, rad = 1, sq = 1;
        __int128 dsum = 0;      /* n' = sum e*n/p */
        int ps[16], es[16], c = 0;
        while (m > 1) {
            uint32_t p = spf[m] ? spf[m] : m;
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            ps[c] = (int)p; es[c] = e; ++c;
        }
        for (int i = 0; i < c; ++i) {
            __int128 p = ps[i], pk = 1, s = 1;
            for (int k = 0; k < es[i]; ++k) { pk *= p; s += pk; }
            sigma *= s;
            __int128 pe1 = 1;
            for (int k = 1; k < es[i]; ++k) pe1 *= p;
            psi *= pe1 * (p + 1);
            rad *= p;
            sq *= (p + 1);
            dsum += (__int128)es[i] * ((__int128)n / p);
        }
        __int128 a342 = 0;
        for (int i = 0; i < c; ++i)
            a342 += (__int128)es[i] * (rad / ps[i]);
        __int128 a001 = sigma + dsum - 2 * (__int128)n;
        __int128 a753 = sigma + psi - 2 * (__int128)n;
        if (a342 == 0) continue;
        if (a001 % a342 != 0) continue;
        if (a001 * sq == a342 * a753) {
            if (A345004_nsmall < 32) A345004_small[A345004_nsmall++] = n;
        }
    }
    free(spf);
    A345004_ready = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!A345004_ready) A345004_init();
    if (n <= (idx_t)A345004_nsmall)
        return (seq_t)A345004_small[n - 1];
    idx_t k = n - (idx_t)A345004_nsmall - 1;
    if (k >= sizeof(A345004_exps) / sizeof(A345004_exps[0])) return 0;
    int p = A345004_exps[k];
    seq_t two_p = (seq_t)1;
    for (int i = 0; i < p; ++i) two_p *= 2;
    seq_t M = two_p - 1;
    return (two_p / 2) * M;
}
