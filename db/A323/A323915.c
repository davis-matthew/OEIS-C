/*
 * A323915  a(n) = A023900(A005940(1+n)).
 * https://oeis.org/A323915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A023900(A005940(1+n)) = Product_{p | A005940(1+n)} (1-p);
 *            only the distinct primes of A005940(1+n) are used, read off the
 *            binary digits of n
 * Range:     i8 n <= 83, i16 n <= 1359, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9635 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static unsigned primes[80];
    static int init = 0;
    if (!init) {
        int c = 0;
        unsigned x = 2;
        while (c < 80) {
            bool ok = true;
            for (int j = 0; j < c && primes[j] * primes[j] <= x; j++)
                if (x % primes[j] == 0) { ok = false; break; }
            if (ok) primes[c++] = x;
            x++;
        }
        init = 1;
    }

    bool used[80];
    for (int k = 0; k < 80; k++) used[k] = false;

    idx_t m = n;
    int i = 1;
    seq_t result = 1;
    while (m > 0) {
        if (m & 1) {
            if (!used[i - 1]) {
                used[i - 1] = true;
                result *= ((seq_t)1 - (seq_t)primes[i - 1]);
            }
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return result;
}
