/*
 * A333570  Number of nonnegative values c such that c^n == -c (mod n).
 * https://oeis.org/A333570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ c ∈ [0,n-1] : c^n + c ≡ 0 (mod n) }|
 * Range:     u8 n <= 414, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 8718 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (Number of nonnegative values c such that c^n == -c (mod n)). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but guard */
    seq_t count = 0;
    const seq_t mod = (seq_t)n;
    for (idx_t c = 0; c < n; ++c) {
        seq_t pow = powmod((seq_t)c, n, mod); /* c^n mod n */
        if ((pow + (seq_t)c) % mod == 0) {
            ++count;
        }
    }
    return count;
}
