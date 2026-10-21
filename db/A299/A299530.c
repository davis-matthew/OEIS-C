/*
 * A299530  Number of regular-faced convex polyhedra (excluding prisms and
 *          antiprisms) with exactly n types of faces.
 * https://oeis.org/A299530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 if n==1; 45 if n==2; 38 if n==3; 17 if n==4; 0
 *            otherwise
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts Platonic (5) + Archimedean (10,3) + Johnson (5,35,35,17) solids by number of face types. */
seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)10;   /* 5 Platonic + 5 Johnson */
    if (n == 2u) return (seq_t)45;   /* 10 Archimedean + 35 Johnson */
    if (n == 3u) return (seq_t)38;   /* 3 Archimedean + 35 Johnson */
    if (n == 4u) return (seq_t)17;   /* 17 Johnson */
    return (seq_t)0;
}
