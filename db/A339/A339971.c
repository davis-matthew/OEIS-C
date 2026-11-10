/*
 * A339971  Odd part of A339821(n).
 * https://oeis.org/A339971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   if 4n = 2^e1 + ... + 2^ek (distinct), a(n) = Product_i
 *            A057023(e_i), where A057023(e) = odd part of prime(e)-1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A339971_oddpm1(unsigned e)
{
    /* odd part of prime(e)-1, by trial-division search for the e-th prime */
    unsigned count = 0;
    uint64_t p = 1;
    while (count < e) {
        p++;
        bool_dummy:;
        {
            int ok = 1;
            uint64_t d;
            for (d = 2; d * d <= p; d++) {
                if (p % d == 0) { ok = 0; break; }
            }
            if (ok) count++;
        }
    }
    p -= 1;
    while ((p & 1ULL) == 0) p >>= 1;
    return p;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    unsigned k = 0;
    uint64_t m = (uint64_t)n;
    while (m) {
        if (m & 1ULL) {
            result *= (seq_t)A339971_oddpm1(k + 2);
        }
        m >>= 1;
        k++;
    }
    return result;
}
