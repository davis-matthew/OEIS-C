/*
 * A378297  Squarefree semiprimes k that remain squarefree semiprimes for
 *          exactly two iterations of the map k -> 2*k+1.
 * https://oeis.org/A378297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A378297: Squarefree semiprimes k that remain squarefree semiprimes
 * for exactly two iterations of k → 2*k+1.
 * The function performs a straightforward search: k, 2k+1, 4k+3
 * must all be square‑free semiprimes, and 8k+7 must not be a
 * square‑free semiprime.  No memoisation is used.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Test whether n is a product of two distinct primes (square‑free semiprime). */
static bool is_semiprime_uint64(uint64_t n)
{
    /* n must be >1 */
    if (n < 6) return false;        /* 6 = 2 * 3 is the smallest semiprime */
    uint64_t pr[16], ex[16];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    if (cnt != 2) return false;
    return (ex[0] == 1 && ex[1] == 1);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t count = 0;
    uint64_t k = 2;

    while (count < (uint64_t)n) {
        if (is_semiprime_uint64(k)) {
            uint64_t a1 = k * 2 + 1;
            if (!is_semiprime_uint64(a1)) {
                ++k;
                continue;
            }
            uint64_t a2 = k * 4 + 3;
            if (!is_semiprime_uint64(a2)) {
                ++k;
                continue;
            }
            uint64_t a3 = k * 8 + 7;
            if (is_semiprime_uint64(a3)) {
                ++k;
                continue;
            }
            /* k satisfies the definition */
            ++count;
            if (count == (uint64_t)n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    return 0;  /* should never be reached */
}
