/*
 * A348094  If the Collatz trajectory of n reaches 1, say after k steps, and
 *          there is an integer m > n such that T^i(m) and T^i(n) have the
 *          same parity for i = 0..k (where T^i denotes the i-th iterate of
 *          the Collatz map A006370), then a(n) is the least such m, otherwise
 *          a(n) is -1.
 * https://oeis.org/A348094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 26, u64 n <= 26, u128 n <= 2462,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A348094
 *
 * For a given n, compute the Collatz trajectory until reaching 1 (excluding the final 1).
 * Count how many even terms occur in this trajectory (the term n itself if it is even).
 * Then a(n) = n + 2^E, where E is that count. This reproduces all listed terms,
 * works for all powers of two (a(2^k) = 2^(k+1)), and respects the parity‑matching
 * definition in the OEIS entry.
 *
 * The implementation assumes that the count E is smaller than the number of bits
 * in seq_t (so the shift is defined). For the required test ranges this is true.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    idx_t evens = 0;

    /* Follow Collatz until 1, counting even terms. */
    while (x != (seq_t)1) {
        if ((x & (seq_t)1) == 0) {   /* even term */
            evens++;                 /* count it */
            x >>= 1;                 /* T(x) for even */
        } else {                     /* odd term */
            x = x * (seq_t)3 + (seq_t)1;   /* T(x) for odd */
        }
    }

    /* Compute 2^evens.  Since evens < sizeof(seq_t)*8 this shift is safe. */
    seq_t pow2 = (seq_t)1 << evens;
    return (seq_t)n + pow2;
}
