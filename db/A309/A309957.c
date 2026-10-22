/*
 * A309957  Product of digits of (n written in base 6).
 * https://oeis.org/A309957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 826, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309957: product of the base‑6 digits of n. */
seq_t a(idx_t n) {
    /* n == 0 has product 0 by definition. */
    if (n == 0) return 0;

    seq_t prod = 1;
    idx_t temp = n;

    /* multiply the base‑6 digits together. */
    while (temp > 0) {
        unsigned digit = temp % 6;      /* digit ∈ {0,…,5} */
        prod *= (seq_t)digit;
        temp /= 6;
    }

    return prod;
}
