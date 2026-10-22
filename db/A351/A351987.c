/*
 * A351987  Numbers with factorial base expansion digits in nonincreasing
 *          order.
 * https://oeis.org/A351987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 2056, u32 n <= 6910, u64 n <= 6910,
 *            u128 n <= 6910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A351987: Numbers with factorial base expansion digits in nonincreasing order.
   a(1) = 0, a(n) = the n-th such number.  */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 0; ; ++val) {
        if (val == 0) {          /* 0 is a valid term */
            ++count;
            if (count == n) return (seq_t)0;
            continue;
        }

        /* convert val to factorial base digits */
        const int MAX_DIGITS = 35;   /* enough for 64‑bit numbers */
        idx_t digits[MAX_DIGITS];
        idx_t len = 0;
        idx_t tmp = val;
        while (tmp > 0 && len < MAX_DIGITS) {
            digits[len] = tmp % (len + 1);
            tmp /= (len + 1);
            ++len;
        }

        /* check nonincreasing: digits[len-1] ≥ … ≥ digits[0] */
        int ok = 1;
        for (idx_t i = 1; i < len; ++i) {
            if (digits[i-1] > digits[i]) { ok = 0; break; }
        }

        if (ok) {
            ++count;
            if (count == n) return (seq_t)val;
        }
    }
}
