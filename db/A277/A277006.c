/*
 * A277006  a(n) = A005940(1+A003714(n)); permutation of squarefree numbers.
 * https://oeis.org/A277006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005940(1+A003714(n)), where A005940(m) is the Doudna
 *            sequence and A003714 gives the fibbinary numbers
 * Range:     u8 n <= 31, u16 n <= 230, u32 n <= 6759, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 93 terms through n = 9971 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 A003714_z(idx_t n) {
    uint64_t f[92];
    int k, i;
    uint64_t rem = (uint64_t)n;
    unsigned __int128 result = 0;
    f[0] = 1;
    f[1] = 2;
    k = 2;
    while (k < 92 && f[k - 1] <= rem) {
        f[k] = f[k - 1] + f[k - 2];
        k++;
    }
    for (i = k - 1; i >= 0; i--) {
        result = result * 2;
        if (f[i] <= rem) {
            rem -= f[i];
            result = result + 1;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    static uint64_t primes[130];
    static int np = 0;
    if (np == 0) {
        uint64_t c = 2;
        while (np < 130) {
            int ok = 1;
            uint64_t d;
            for (d = 2; d * d <= c; d++) {
                if (c % d == 0) { ok = 0; break; }
            }
            if (ok) primes[np++] = c;
            c++;
        }
    }
    unsigned __int128 m = A003714_z(n); /* 1+A003714(n) - 1 */
    seq_t result = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            result *= (seq_t)primes[i - 1];
        } else {
            i++;
        }
        m >>= 1;
    }
    return result;
}
