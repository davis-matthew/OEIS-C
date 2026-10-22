/*
 * A363377  Largest positive integer having n holes that can be made using the
 *          fewest possible digits.
 * https://oeis.org/A363377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (89*10^((n-1)/2)-8)/9 if n is odd; a(n) =
 *            8*(10^(n/2)-1)/9 if n is even (n>=2); a(0) = 7, a(1) = 9
 *            (Numbers grow as 10^((n-1)/2) for odd n, 10^(n/2) for even n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)9;

    if ((n & 1) == 0) {          /* even n >= 2 */
        idx_t k = n >> 1;        /* n/2 */
        seq_t p = pow10(k);      /* 10^k */
        seq_t res = (p - 1);     /* 10^k - 1 */
        res = res * (seq_t)8;    /* *8 */
        res = res / (seq_t)9;    /* /9 */
        return res;
    } else {                     /* odd n >= 3 */
        idx_t k = (n - 1) >> 1;  /* (n-1)/2 */
        seq_t p = pow10(k);      /* 10^k */
        seq_t res = (p * (seq_t)89) - (seq_t)8; /* 89*10^k - 8 */
        res = res / (seq_t)9;    /* /9 */
        return res;
    }
}
