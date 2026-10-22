/*
 * A325692  "Sloping senary numbers": write numbers in senary (that is, base
 *          6) under each other (right-justified), read diagonals in upward
 *          direction, convert to decimal.
 * https://oeis.org/A325692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k≥0, n+k ≥ 6^k} ((n+k)/6^k mod 6) * 6^k (sum of base‑6
 *            digits found on the diagonal r – c = n, read bottom‑to‑top; each
 *            digit occupies 6^k in the result)
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;                     /* accumulated result (base 10)   */
    seq_t pow6 = 1;                    /* current power 6^k (k starts at 0) */
    const seq_t limit64 = (seq_t)((uint64_t)-1);   /* max value representable with idx_t */

    for (idx_t k = 0; ; ++k) {
        /* r = n + k  (cast to seq_t to allow larger values than idx_t) */
        seq_t r = (seq_t)n + (seq_t)k;

        if (r < pow6) break;           /* no further digits on this diagonal */

        seq_t digit = (r / pow6) % (seq_t)6;
        res += digit * pow6;           /* append the digit at position k */

        /* protect against overflow of pow6 when seq_t is 64‑bit */
        if (pow6 > limit64 / 6) break; /* next power would overflow */
        pow6 *= 6;                     /* move to next digit position   */
    }
    return res;
}
