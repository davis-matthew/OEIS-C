/*
 * A313057  Coordination sequence Gal.6.132.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A313057 – Coordination sequence Gal.6.132.3
 *
 * The sequence follows a simple linear recurrence after the first two terms:
 *   a(0) = 1
 *   a(1) = 4
 *   for n >= 2:
 *       a(n) = a(n-1) + 4  if n mod 12 is 0, 1, 4 or 9
 *       a(n) = a(n-1) + 5  otherwise
 *
 * An efficient closed‑form implementation computes the number of “4‑steps”
 * in the range [2, n] by using the 12‑periodic pattern.
 *
 * Complexity: O(1) time, O(1) memory.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    idx_t len = n - 1;                 /* count of indices from 2 to n */
    idx_t full_cycles = len / 12;
    idx_t rem = len % 12;

    /* a(1) already accounted for */
    seq_t result = (seq_t)4;

    /* Contribution from complete 12‑step cycles */
    result += (seq_t)full_cycles * (seq_t)56;   /* 4*4 + 8*5 = 56 */

    /* Partial cycle (at most 11 indices) */
    idx_t start = 2;
    for (idx_t i = 0; i < rem; ++i) {
        idx_t k = start + i;
        seq_t inc = ((k % 12) == 0 || (k % 12) == 1 ||
                     (k % 12) == 4 || (k % 12) == 9) ? (seq_t)4 : (seq_t)5;
        result += inc;
    }

    return result;
}
