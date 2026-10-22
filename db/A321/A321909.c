/*
 * A321909  a(n) is the least base b > 1 in which the additive persistence of
 *          n is <= 1.
 * https://oeis.org/A321909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least base b > 1 such that the sum of the digits of
 *            n in base b is less than b
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to: a(n) = min{b > 1 | digit_sum(n, b) < b}.  The base n itself always works
   (n = 1·b + 0, digit sum = 1 < b).  Therefore a(n) ≤ n and an early stop is usually reached. */

seq_t a(idx_t n)
{
    /* The trivial cases n = 0 or 1 both have a(0) = a(1) = 2 */
    if (n <= 1) {
        return (seq_t)2;
    }

    /* Search increasing bases from 2 upwards until the digit‑sum condition is satisfied.
       For each base we compute the digit sum in linear time with respect to the number of digits.
       We stop the digit sum loop early if the sum already reaches or exceeds the base. */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t tmp = n;
        idx_t sum = 0;

        while (tmp > 0) {
            sum += tmp % b;
            if (sum >= b) {       /* cannot satisfy the condition any more */
                break;
            }
            tmp /= b;
        }

        if (sum < b) {
            return (seq_t)b;
        }
    }

    /* In theory the loop always returns at b = n,
       but this fallback provides a safe return value for any malformed input. */
    return (seq_t)(n + 1);
}
