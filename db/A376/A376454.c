/*
 * A376454  n in base whose place values are a modified ternary sequence; see
 *          Comments.
 * https://oeis.org/A376454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = mixed‐radix representation of n in the modified ternary
 *            base defined by the place values (1,2,3,9,27,81,…). Each digit
 *            is 0, 1 or 2, and the value is the decimal integer composed of
 *            those digits (e.g. 210 for 8)
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* trivial case */
        return (seq_t)0;

    /* Build the list of weights up to n: 1, 2, and 3^k (k ≥ 1). */
    idx_t weights[64];
    idx_t wcnt = 0;
    weights[wcnt++] = 1;
    weights[wcnt++] = 2;

    idx_t pow3 = 3;
    while (wcnt < 64 && pow3 <= n) {
        weights[wcnt++] = pow3;
        if (pow3 > n / 3)  /* avoid overflow of pow3 * 3 */
            break;
        pow3 *= 3;
    }

    /* Produce the mixed‑radix digits from most significant to least
       significant, building the decimal integer from those digits. */
    seq_t result = 0;
    for (idx_t idx = wcnt; idx > 0;) {
        idx--;                            /* handle descending loop safely */
        idx_t weight = weights[idx];
        idx_t digit  = n / weight;
        n -= digit * weight;
        result = result * 10 + digit;
    }

    return result;
}
