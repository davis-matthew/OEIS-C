/*
 * A357073  For n >= 1, a(n) = A003714(n) mod n.
 * https://oeis.org/A357073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003714(n) mod n, where A003714(n) = sum of 2^(j-2) over
 *            the Zeckendorf parts F(j) of n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A357073_fibbinary_mod(idx_t n) {
    uint64_t f[92];
    int k;
    int i;
    uint64_t rem = (uint64_t)n;
    unsigned __int128 m = (unsigned __int128)n;
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
        result %= m;
    }
    return (seq_t)(uint64_t)result;
}

seq_t a(idx_t n) {
    return A357073_fibbinary_mod(n);
}
