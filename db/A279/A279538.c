/*
 * A279538  a(n) = -n^3 + 70*n^2 - 939*n + 2393.
 * https://oeis.org/A279538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^3 + 70*n^2 - 939*n + 2393
 * Range:     i8 none, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;          /* cast idx_t to seq_t */
    seq_t NN = N * N;     /* n^2 */
    seq_t NNN = NN * N;   /* n^3 */
    seq_t result = -NNN + 70 * NN - 939 * N + 2393;
    return result;
}
