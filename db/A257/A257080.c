/*
 * A257080  n multiplied by the least nonzero digit missing from its factorial
 *          base representation: a(n) = n * A257079(n).
 * https://oeis.org/A257080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n * A257079(n) where A257079(n) is the least nonzero
 *            digit missing from the factorial-base representation of n
 * Range:     u8 n <= 66, u16 n <= 8181, u32 n <= 8181, u64 n <= 8181,
 *            u128 n <= 8181, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly – product will be 0 anyway. */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t tmp = n;
    idx_t i = 1;                     /* current factorial position (1‑based) */
    unsigned char seen[65] = { 0 };  /* digits seen so far, index 0..64 */
    idx_t max_digit = 0;

    /* Compute the factorial representation of n. */
    while (tmp > 0) {
        idx_t digit = tmp % (i + 1);    /* digit for position i */
        tmp /= (i + 1);
        if (digit <= 64) {
            seen[(unsigned int)digit] = 1;
        }
        if (digit > max_digit) {
            max_digit = digit;
        }
        ++i;
    }

    /* Find the smallest nonzero digit that is missing. */
    idx_t missing = max_digit + 1;   /* default if all digits 1..max_digit are present */
    for (idx_t d = 1; d <= max_digit; ++d) {
        if (!seen[(unsigned int)d]) {
            missing = d;
            break;
        }
    }

    return (seq_t)n * (seq_t)missing;
}
