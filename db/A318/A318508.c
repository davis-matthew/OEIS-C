/*
 * A318508  a(n) = A032742(n) AND A001065(n)-A032742(n), where AND is
 *          bitwise-and (A004198) and A001065 = sum of proper divisors and
 *          A032742 = the largest proper divisor of n.
 * https://oeis.org/A318508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A032742(n) AND (A001065(n)-A032742(n))
 * Range:     u8 n <= 515, u16 n <= 9741, u32 n <= 9741, u64 n <= 9741,
 *            u128 n <= 9741, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A032742(n)   : largest proper divisor of n (n>1)   */
/*   A001065(n)   : sum of proper divisors of n         */
/*   AND          : bitwise AND of the two numbers     */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n = 0 or 1 directly. */
    if (n <= 1) return 0;

    /* --- compute smallest prime factor (spf) --- */
    idx_t spf = n;
    idx_t i;
    for (i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            spf = i;
            break;
        }
    }

    /* largest proper divisor = n / spf */
    seq_t lpd = (seq_t)(n / spf);          /* fits in seq_t for n up to ~2^64 */

    /* --- compute sum of all divisors of n (sigma(n)) --- */
    seq_t sum = 0;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(n / i);
            sum += d1;
            if (d2 != d1) sum += d2;
        }
    }

    /* sum of proper divisors = sigma(n) - n */
    seq_t proper_sum = sum - (seq_t)n;

    /* difference required in the definition */
    seq_t diff = proper_sum - lpd;   /* unsigned underflow is impossible for n≥2 */

    /* final value: bitwise AND */
    return lpd & diff;
}
