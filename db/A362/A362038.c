/*
 * A362038  A list of lists where T(n,k) is the smallest n-digit number whose
 *          digits have arithmetic mean k, for 1 <= k <= 9.
 * https://oeis.org/A362038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   Let r = (n-1)/9 and k = (n-1)%9+1. a(n) is the smallest
 *            (r+1)-digit decimal number whose digits sum to (r+1)*k. The
 *            number is constructed by a greedy allocation of digits: at each
 *            position choose the smallest digit that still allows the
 *            remaining positions to reach the required sum
 * Range:     u8 n <= 24, u16 n <= 44, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out‑of‑range guard */

    /* Determine the row (number of digits) and column (k). */
    idx_t row = (n - 1) / 9;          /* 0‑based row index */
    idx_t k   = (n - 1) % 9 + 1;      /* 1‑based k value */
    idx_t num_digits = row + 1;       /* actual number of digits */

    /* Target digit sum S = num_digits * k. */
    seq_t S = (seq_t)num_digits * (seq_t)k;
    seq_t rem = S;                    /* remaining sum to allocate */
    seq_t result = 0;
    const seq_t TEN = (seq_t)10;

    /* Construct number digit by digit. */
    for (idx_t pos = 1; pos <= num_digits; ++pos) {
        idx_t remaining_digits = num_digits - pos;
        seq_t nine_rem = (seq_t)9 * remaining_digits;

        /* Required minimal digit at this position */
        seq_t require = 0;
        if (rem > nine_rem) {
            require = rem - nine_rem;
        }
        unsigned digit;
        if (pos == 1) {
            /* Leading digit must be at least 1 */
            digit = (require < 1) ? 1 : (unsigned)require;
        } else {
            digit = (unsigned)require;  /* 0‑based for other positions */
        }
        /* Append digit to result */
        result = result * TEN + (seq_t)digit;
        rem -= digit;
    }
    return result;
}
