/*
 * A325645  "Sloping quinary numbers": write numbers in quinary under each
 *          other (right-justified), read diagonals in upward direction,
 *          convert to decimal.
 * https://oeis.org/A325645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal value of the base‑5 digits read from the
 *            n‑th anti‑diagonal of the right‑justified quinary
 *            representations of the integers. For each i ≥ n such that i – n
 *            < number of base‑5 digits of i, the digit at position i – n in i
 *            (counting from the least significant digit) is written; t...
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precompute powers of 5 up to 5^19 – more than enough for n ≤ 10 000 */
    static seq_t pow5[20];
    static int pow5_ready = 0;
    if (!pow5_ready) {
        pow5[0] = 1;
        for (int i = 1; i < 20; ++i)
            pow5[i] = pow5[i-1] * (seq_t)5;
        pow5_ready = 1;
    }

    /* find the largest i for which i-n < digits_base5(i) */
    idx_t i = n;
    for (;;) {
        idx_t j = i - n;
        /* count base‑5 digits of i */
        idx_t tmp = i;
        unsigned digs = 0;
        do { ++digs; tmp /= 5; } while (tmp);
        if (j >= digs) break;   /* condition fails – stop */
        ++i;
    }
    idx_t max_i = i - 1;

    /* accumulate digits from largest i down to n */
    seq_t result = 0;
    if (max_i >= n) {
        /* Safe because max_i >= n and j = i-n is always < 20 */
        for (idx_t k = max_i;; --k) {
            idx_t j = k - n;
            seq_t digit = ((seq_t)k / pow5[j]) % (seq_t)5;
            result = result * (seq_t)5 + digit;
            if (k == n) break;
        }
    }
    return result;
}
