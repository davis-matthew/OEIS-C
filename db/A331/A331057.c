/*
 * A331057  Coordination sequence for 10-valent vertex in 1-skeleton of
 *          disdyakis triacontahedron.
 * https://oeis.org/A331057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0) = 1 a(1) = a(2) = a(4) = a(5) = 10 a(3) = 20 a(6) = 1 a(n)
 *            = 0 for n > 6 These are the only non‑zero terms of the
 *            coordination sequence of a 10‑valent vertex in the 1‑skeleton of
 *            the disdyakis triacontahedron
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)          return (seq_t)1;
    if (n == 6u)          return (seq_t)1;
    if (n == 3u)          return (seq_t)20;
    if (n == 1u || n == 2u || n == 4u || n == 5u)
        return (seq_t)10;
    return (seq_t)0;
}
