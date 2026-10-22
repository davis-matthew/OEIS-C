/*
 * A336430  Number of partitions of n into two positive integer parts that
 *          have the same number of decimal digits.
 * https://oeis.org/A336430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k=1}^{⌊n/2⌋} [digits(k) = digits(n−k)]
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into two positive parts that have the same number of decimal digits. */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t half = n >> 1;          /* floor(n/2) */
    idx_t pow10 = 1;              /* 10^(d-1), the smallest d‑digit number */

    while (pow10 <= half) {
        idx_t low  = pow10;                /* smallest d‑digit number */
        idx_t high = pow10 * 10ULL - 1ULL; /* largest d‑digit number */

        /* Lower bound for k: must be >= low and >= n-high (so that n-k has d digits). */
        idx_t L = low;
        if (n > high) {
            idx_t tmp = n - high;
            if (tmp > L) L = tmp;
        }

        /* Upper bound for k: must be <= high, <= n-low (so n-k >= low), and <= half. */
        idx_t R = high;
        if (n > low) {
            idx_t tmp = n - low;
            if (tmp < R) R = tmp;
        }
        if (half < R) R = half;

        if (R >= L) {
            ans += (seq_t)(R - L + 1);
        }

        /* Advance to next digit length; loop stops when pow10 > half. */
        pow10 *= 10ULL;
    }

    return ans;
}
