/*
 * A330555  Coordination sequence for 1-skeleton of icosidodecahedron.
 * https://oeis.org/A330555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(1)=4, a(2)=a(3)=a(4)=8, a(5)=1, a(n)=0 for n>5
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)                 return (seq_t)1;
    if (n == 1ULL)                 return (seq_t)4;
    if (n >= 2ULL && n <= 4ULL)    return (seq_t)8;
    if (n == 5ULL)                 return (seq_t)1;
    return (seq_t)0;
}
