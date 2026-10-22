/*
 * A340445  Number of partitions of n into 3 parts that are not all the same.
 * https://oeis.org/A340445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of partitions of n into exactly 3 positive parts that
 *            are NOT all equal. p3(n) (partitions into 3 parts) is p3(n) =
 *            Σ_{c=1..⌊n/3⌋} (⌊(n‑3c)/2⌋ + 1) The required term is a(n) =
 *            p3(n) – [n ≡ 0 (mod 3)] (the triple‑equal partition is removed)
 * Range:     u8 n <= 55, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0,1,2 have no 3‑part partitions */
    if (n < 3) {
        return (seq_t)0;
    }

    idx_t m = n / 3;              /* floor(n/3) */
    seq_t sum = (seq_t)0;
    idx_t r = n % 3;              /* n mod 3 */

    for (idx_t j = 0; j < m; ++j) {
        /* floor((3*j + r)/2) */
        seq_t t = ((seq_t)3 * j + r) / (seq_t)2;
        sum += t;
    }

    /* Each term contributed +1 in the formula for p3(n) */
    seq_t p3 = sum + (seq_t)m;

    /* Subtract the triple‑equal partition when n is divisible by 3 */
    if (r == 0) {
        /* p3 is at least 1 for n ≥ 3, so the subtraction is safe */
        p3 -= (seq_t)1;
    }

    return p3;
}
