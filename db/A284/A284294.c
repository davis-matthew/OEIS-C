/*
 * A284294  Numbers using only digits 1 and 9.
 * https://oeis.org/A284294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1534, u64 n <= 8208,
 *            u128 n <= 8208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284294: numbers composed only of digits 1 and 9
 * in increasing order. This implementation computes the n-th
 * term by determining the length L of the number and the
 * binary pattern that represents the digits 1 (for 0) and 9 (for 1). */
seq_t a(idx_t n)
{
    /* Find smallest k such that 2^k >= n+2. */
    idx_t p = n + 2;
    idx_t power = 1;   /* power = 2^k */
    int k = 0;
    while (power < p) {
        power <<= 1;
        k++;
    }
    /* Length of the number */
    int L = k - 1;
    /* Count of all numbers with length < L */
    idx_t C_prev = (power >> 1) - 2;   /* 2^L - 2 */
    /* Index within the block of length L (0‑based) */
    idx_t idx0 = n - C_prev - 1;

    seq_t result = 0;
    for (int i = L - 1; i >= 0; --i) {
        result = result * 10 + (((idx0 >> i) & 1) ? 9 : 1);
    }
    return result;
}
