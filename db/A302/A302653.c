/*
 * A302653  Number of minimum total dominating sets in the n-cycle graph.
 * https://oeis.org/A302653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is odd; 4 if n % 4 == 0; (n/2)^2 if n % 4 == 2
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;
    }
    /* Odd n: return n */
    if (n & 1ULL) {
        return (seq_t)n;
    }
    /* Even n: decide by n modulo 4 */
    switch (n & 3ULL) {
        case 0: /* n ≡ 0 (mod 4) */
            return (seq_t)4;
        case 2: /* n ≡ 2 (mod 4) */
        {
            seq_t half = (seq_t)(n / 2);
            return half * half;
        }
        default:
            /* Should never reach here for n ≥ 1 */
            return (seq_t)0;
    }
}
