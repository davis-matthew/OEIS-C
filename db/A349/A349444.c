/*
 * A349444  Dirichlet convolution of A003602 (a Kimberling paraphrase) with
 *          A092673 (Dirichlet inverse of A001511).
 * https://oeis.org/A349444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d|n} A003602(n/d) * A092673(d) It can be shown that
 *            a(n) = 0 if 4 | n a(1) = 1 a(n) = φ(n)/2 if n is odd and n>1
 *            a(n) = -φ(n)/2 if n ≡ 2 (mod 4), n > 2 a(2) = -1 where φ is
 *            Euler's totient. The function below computes φ(n) by factoring n
 *            with trial‑division and applies the above case...
 * Range:     i8 n <= 256, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9235 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t is a signed integer type, idx_t is unsigned (uint64_t). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset does not matter */
    if (n == 1) return 1;

    if ((n & 3) == 0)                /* n % 4 == 0 -> a=0 */
        return 0;

    seq_t phi = totient(n);

    if ((n & 1) == 1) {              /* n odd, n>1 */
        return phi / 2;
    }

    /* n is even and not divisible by 4 (n % 4 == 2) */
    if ((phi & 1) != 0)              /* only for n == 2   */
        return -phi;                 /* -φ(n) */
    return -(phi / 2);               /* -φ(n)/2 for n>2 */
}
