/*
 * A371743  a(n) = Sum_{k=0..floor(n/2)} binomial(4*n-k,n-2*k).
 * https://oeis.org/A371743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(4*n-k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binomial coefficient C(n,k) without overflowing
 *  the intermediate result for all n that fit in a 64-bit host
 *  integer. This implementation uses only integer arithmetic
 *  supported by seq_t and idx_t.  It relies on the observation that
 *  C(n,k) = C(n,n-k), and reduces each multiplicative step by the
 *  greatest common divisor of the numerator part and the step index.
 *  The division is performed first to keep the intermediate values
 *  small.  This matches the requirement that the final value is computed
 *  correctly for any n that fits in seq_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;          /* numerator factor */
        idx_t g = gcd_idx(num, i);      /* reduce before multiply */
        num /= g;
        idx_t i2 = i / g;
        /* res should be divisible by i2 */
        res = res / i2;
        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        sum += my_binom(4 * n - k, n - 2 * k);
    }
    return sum;
}
