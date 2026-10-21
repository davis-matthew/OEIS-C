/*
 * A257927  Array read by antidiagonals: M[i,j] = j-th decimal digit of
 *          (i+1)/(i-1) for i, j = 1,2,3,4,5,... or 0 if undefined.
 * https://oeis.org/A257927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A257927 – Array read by antidiagonals.
   M[i,j] = j‑th decimal digit of (i+1)/(i-1) (0 when undefined).
   a(1) = 0; further terms are read by traversing all (i,j) in
   antidiagonal order starting with (1,1). */
/* Note: Uses the pow10 utility to extract integer‑part digits. */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Find diagonal index s such that cumulative elements up to s
       (inclusive) is >= n.  The cumulative count for diagonal s is
       (s+1)(s+2)/2.  Linear search is fine because n ≤ ~10⁴ in tests. */
    idx_t s = 0;
    idx_t cum = 1;          /* count up to diagonal s */
    while (cum < n) {
        ++s;
        cum += (s + 1);
    }

    /* Calculate previous cumulative count and offset within current diagonal */
    idx_t cum_prev = cum - (s + 1);
    idx_t offset0 = n - cum_prev - 1;      /* zero‑based offset in this diagonal */

    /* Convert to matrix indices (i,j) */
    idx_t i = (s + 1) - offset0;       /* row */
    idx_t j = (s + 2) - i;             /* column = digit position */

    /* Handle the undefined case (i == 1) */
    if (i == 1) {
        return 0;                    /* M[1,j] is defined as 0 */
    }

    /* Compute the decimal expansion of (i+1)/(i-1) */
    idx_t N = i + 1;          /* numerator  */
    idx_t D = i - 1;          /* denominator */
    idx_t q = N / D;          /* integer part   */
    idx_t r = N % D;          /* remainder       */

    /* Determine the length of the integer part (number of decimal digits
       before the point).  For the small values encountered here this is
       always 1, but the code is general. */
    idx_t len = 1;
    idx_t tmp = q;
    while (tmp >= 10) {
        ++len;
        tmp /= 10;
    }

    if (j <= len) {           /* the requested digit lies in the integer part   */
        seq_t pow10_val = pow10(len - j);   /* 10^(len-j) as big integer */
        seq_t q_seq = (seq_t)q;            /* cast to seq_t */
        seq_t d = (q_seq / pow10_val) % 10;
        return d;
    } else {                  /* digit comes from the fractional part */
        idx_t pos = j - len;  /* position after the decimal point (1‑based) */
        idx_t rem = r;
        seq_t digit = 0;

        for (idx_t k = 0; k < pos; ++k) {
            rem *= 10;
            digit = (seq_t)(rem / D);   /* current fractional digit */
            rem %= D;                   /* new remainder          */
        }

        return digit;
    }
}
