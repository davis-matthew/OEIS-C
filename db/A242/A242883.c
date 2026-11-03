/*
 * A242883  Numbers n such that (n^n+3^3)/(n+3) is an integer.
 * https://oeis.org/A242883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 77 terms through n = 392 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that (n^n+3^3)/(n+3) is an integer. */
/* a(n) = the n‑th such integer (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* Compute the n‑th term of A242883. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    idx_t found = 0;
    for (idx_t cand = 1;; ++cand) {
        seq_t mod   = (seq_t)(cand + 3);          /* (n + 3) */
        seq_t pow_n = powmod((seq_t)cand, cand, mod); /* n^n mod (n+3) */
        seq_t sum   = pow_n + (seq_t)27;          /* n^n + 27 */
        if ((sum % mod) == 0) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}
