/*
 * A386799  Numbers without an exponent 3 in their prime factorization.
 * https://oeis.org/A386799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9324 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: numbers whose prime factorizations have no exponent 3 (sequence A386799) */
#include <stdbool.h>
#include <stdint.h>

/* Check whether an integer x has any prime exponent equal to 3 in its factorization. */
static bool _is_semi3_free(idx_t x)
{
    if (x < 2) return true;          /* 1 is allowed */
    idx_t n = x;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (exp == 3) return false;   /* exponent 3 found */
        }
    }
    /* Remaining factor (if any) has exponent 1, which is fine. */
    return true;
}

/* Return the n-th term (1‑based) of the sequence A386799. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, as offset is 1, but keep defensive */

    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (_is_semi3_free(candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }
    return (seq_t)candidate;
}
