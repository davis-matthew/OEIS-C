/*
 * A242427  n! mod n^3.
 * https://oeis.org/A242427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! mod n^3
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute n! modulo n^3 using 128‑bit arithmetic for safety. */
    if (n == 0) return 0;    /* not used for offset 1, but defensive */

    unsigned __int128 mod = (unsigned __int128)n * n * n;
    unsigned __int128 res = 1 % mod;

    for (idx_t i = 2; i <= n; ++i) {
        res = (res * i) % mod;
    }
    return (seq_t)res;
}
