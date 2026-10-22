/*
 * A391991  Triangle read by rows: T(n,k) = number of digits in decimal
 *          representation of binomial(n,k), 0<=k<=n.
 * https://oeis.org/A391991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, base
 * Formula:   a(n) = digits(binomial(r, k)) where r is the unique integer with
 *            r·(r+1)/2 ≤ n < (r+1)·(r+2)/2 and k = n - r·(r+1)/2. Digits are
 *            counted in decimal
 * Range:     u8 n <= 39, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binomial, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/digit_len.h"

/* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t _row_of(idx_t n)
{
    /* Fast search using exponential then binary.  All multiplications
       are performed in seq_t to avoid over‑flow. */
    seq_t N = (seq_t)n;
    idx_t low = 0, high = 1;
    while ((seq_t)high * (high + 1) / 2 <= N)
        high <<= 1;                  /* exponential growth */

    /* Binary search between low+1 and high-1 inclusive */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((seq_t)mid * (mid + 1) / 2 <= N)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = _row_of(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Compute binomial(r,k) and return its digit count */
    seq_t c = binomial(r, k);
    return digit_len(c);
}
