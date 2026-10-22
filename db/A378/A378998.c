/*
 * A378998  Number of trailing 1-bits in the binary representation of
 *          sigma(n).
 * https://oeis.org/A378998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2(1 + sigma(n)) (number of trailing 1‑bits in sigma(n))
 * Range:     u8 n <= 9877, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* Compute sigma(n) */
    seq_t s = sigma_of(n);

    /* Count trailing 1‑bits: equivalent to v2(1 + sigma(n)) */
    seq_t t = s + 1;   /* 1 + sigma(n)  */
    seq_t cnt = 0;     /* result */

    /* While the lowest bit of t is 0, t is divisible by 2 */
    while ((t & 1) == 0) {
        t >>= 1;
        cnt++;
    }
    return cnt;
}
