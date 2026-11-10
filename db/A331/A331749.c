/*
 * A331749  a(n) = A002487(A163511(n)) - A002487(n).
 * https://oeis.org/A331749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A002487(A163511(n)) - A002487(n), where A002487 is
 *            Stern's diatomic series (a(0)=0, a(1)=1, a(2n)=a(n),
 *            a(2n+1)=a(n)+a(n+1)) and A163511(n) = p(A000120(n)) *
 *            Product_{m=1..A000120(n)} p(m)^A163510(n,m)
 * Range:     i8 n <= 130, i16 n <= 1038, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9674 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A331749_u128;

static seq_t A002487_s(seq_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if ((m & 1) != 0) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

static A331749_u128 A002487_u(A331749_u128 m) {
    A331749_u128 x = 1, y = 0;
    while (m > 0) {
        if ((m & 1) != 0) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

static void A331749_primes(uint32_t primes[64]) {
    int pc = 0;
    for (uint32_t c = 2; pc < 64; c++) {
        bool ok = true;
        for (int j = 0; j < pc; j++) {
            if (primes[j] * primes[j] > c) break;
            if (c % primes[j] == 0) { ok = false; break; }
        }
        if (ok) primes[pc++] = c;
    }
}

seq_t a(idx_t n) {
    uint32_t primes[64];
    int gaps[64];
    int cnt = 0, zeros = 0;
    idx_t t = n;

    if (n == 0) {
        /* A163511(0) = 1, A002487(1) = 1, A002487(0) = 0 */
        return (seq_t)1;
    }
    A331749_primes(primes);
    while (t) {
        if (t & 1) { gaps[cnt++] = zeros; zeros = 0; }
        else zeros++;
        t >>= 1;
    }

    /* Try computing A163511(n) in unsigned 128-bit to avoid overflow in narrow seq_t. */
    {
        const A331749_u128 lim = ((A331749_u128)1) << 126;
        A331749_u128 v = (A331749_u128)primes[cnt - 1];
        bool overflow = false;
        for (int i = 0; i < cnt && !overflow; i++) {
            for (int e = 0; e < gaps[i]; e++) {
                if (v > lim / primes[i]) { overflow = true; break; }
                v *= primes[i];
            }
        }
        if (!overflow) {
            __int128 f1 = (__int128)A002487_u(v);
            __int128 f2 = (__int128)A002487_u((A331749_u128)n);
            __int128 d = f1 - f2;
            if (d >= 0) {
                return (seq_t)(long long)d == (seq_t)0 && d != 0 ? (seq_t)0 : (seq_t)(long long)d;
            } else {
                return -(seq_t)(long long)(-d);
            }
        }
    }

    /* Fallback: compute directly in seq_t (arbitrary precision builds). */
    {
        seq_t result = (seq_t)primes[cnt - 1];
        for (int i = 0; i < cnt; i++) {
            for (int e = 0; e < gaps[i]; e++) {
                result *= (seq_t)primes[i];
            }
        }
        return A002487_s(result) - A002487_s((seq_t)n);
    }
}
