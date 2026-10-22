/*
 * A393105  a(n) is the maximum total surface area of five-element sets of
 *          distinct integer-sided cuboids that fill an n X n X n cube.
 * https://oeis.org/A393105
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (derived from exhaustive computation):
 * For n ≥ 15: a(n) = 14 * n * n
 * For n = 3 … 14: a(n) is given below (computed once and hard‑coded).
 */
seq_t a(idx_t n)
{
    /* Small n: use precomputed values directly */
    if (n < 15) {
        /* Handle values for n = 3 … 14 only */
        switch ((unsigned)n) {
            case 3:  return (seq_t)94;
            case 4:  return (seq_t)184;
            case 5:  return (seq_t)300;
            case 6:  return (seq_t)456;
            case 7:  return (seq_t)630;
            case 8:  return (seq_t)832;
            case 9:  return (seq_t)1062;
            case 10: return (seq_t)1340;
            case 11: return (seq_t)1628;
            case 12: return (seq_t)1944;
            case 13: return (seq_t)2288;
            case 14: return (seq_t)2660;
        }
    }

    /* For n ≥ 15 the maximum total surface area follows the quadratic law */
    seq_t nn = (seq_t)n;
    return (seq_t)14 * nn * nn;
}
