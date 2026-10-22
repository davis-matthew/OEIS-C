/*
 * A363166  Bouton numbers: a(n) is the number of P positions in games of Nim
 *          with three nonzero heaps each containing at most n sticks.
 * https://oeis.org/A363166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of triples (a,b,c) with 1 ≤ a < b < c ≤ n and c =
 *            a XOR b
 * Range:     u8 n <= 45, u16 n <= 256, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function iterates over all pairs (a,b), computes c = a^b, and counts those */
/*  where b < c ≤ n.  The algorithm runs in O(n²) time, which is the simplest */
/*  direct method for the required range. */

seq_t a(idx_t n)
{
    if (n < 1)          /* the sequence is defined for n ≥ 1; 0 is unused */
        return 0;

    seq_t count = 0;    /* accumulator in seq_t */

    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = a + 1; b <= n; ++b) {
            idx_t c = a ^ b;              /* compute nim‑sum */
            if (c > b && c <= n)          /* keep a < b < c <= n */
                count += 1;               /* increment seq_t counter */
        }
    }

    return count;
}
