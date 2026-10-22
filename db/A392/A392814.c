/*
 * A392814  Additive with a(p^e) = p mod e.
 * https://oeis.org/A392814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing OEIS sequence A392814
   a(n) = sum_{p^e || n} (p mod e)          */
/* definition of an additive function with
   a(p^e) = p mod e                           */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;   /* a(1) = 0 */

    idx_t tmp = n;
    seq_t sum = 0;

    /* handle factor 2 */
    idx_t cnt = 0;
    while ((tmp & 1ULL) == 0) {  /* tmp % 2 == 0 */
        tmp >>= 1;
        ++cnt;
    }
    if (cnt > 0) {
        sum += (seq_t)(2ULL % cnt);
    }

    /* handle odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            /* cnt > 0 always */
            sum += (seq_t)(p % cnt);
        }
    }

    /* remaining prime factor > 1 (exponent 1 -> contributes 0) */
    /* not needed to add, but kept for completeness */
    /* if (tmp > 1) { } */

    return sum;
}
