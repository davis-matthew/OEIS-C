/*
 * A261011  Positive integers n such that ceiling(n^(1/3)) divides n.
 * https://oeis.org/A261011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer n such that ceil(n^(1/3)) divides n.
 *            Computation: find k such that S(k-1) < n ≤ S(k), where S(k) =
 *            (k+1)*(3k+2)/2. Then set i = n - S(k-1) - 1 (0‑based), and
 *            return k³ + 1 + i·(k+1)
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0) return 0;   /* undefined, but safety */

    idx_t k = 0;
    idx_t count_before = 0;

    /* find the block containing the n‑th element */
    while (count_before + (3 * k + 1) < n) {
        count_before += 3 * k + 1;
        ++k;
    }

    idx_t i = n - count_before - 1;       /* 0‑based index within block */

    seq_t k_seq = (seq_t)k;
    seq_t term = k_seq * k_seq * k_seq + 1 + (seq_t)i * (k_seq + 1);

    return term;
}
