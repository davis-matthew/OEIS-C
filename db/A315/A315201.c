/*
 * A315201  Coordination sequence Gal.5.271.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 * 36 * (n/6) if n % 6 = 0 and n > 0 * 28 *
 *            (n/6) + 6 if n % 6 = 1 * 30 * (n/6) + 10 if n % 6 = 2 * 28 *
 *            (n/6) + 14 if n % 6 = 3 * 30 * (n/6) + 20 if n % 6 = 4 * 28 *
 *            (n/6) + 22 if n % 6 = 5
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 6;          /* integer division */
    idx_t r = n % 6;          /* remainder */

    /* Convert k to the integer type used for the sequence values. */
    seq_t sk = (seq_t)k;

    switch (r) {
        case 0:  return (seq_t)36 * sk;
        case 1:  return (seq_t)28 * sk + 6;
        case 2:  return (seq_t)30 * sk + 10;
        case 3:  return (seq_t)28 * sk + 14;
        case 4:  return (seq_t)30 * sk + 20;
        case 5:  return (seq_t)28 * sk + 22;
    }

    /* Unreachable, but keep compiler happy. */
    return 0;
}
