/*
 * A276860  First differences of the Beatty sequence A276853 for 2*e.
 * https://oeis.org/A276860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) – floor(n*r – r), r = 2*e ≈ 5.43656365691809
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned long long NUM = 5436563656918090470ULL; /* r * 10^18 */
static const unsigned long long DEN = 1000000000000000000ULL; /* 10^18 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – safety for n=0 */
    unsigned __int128 t1 = (unsigned __int128)n * NUM;
    seq_t floor1 = (seq_t)(t1 / DEN);
    unsigned __int128 t2 = (unsigned __int128)(n - 1) * NUM;
    seq_t floor2 = (seq_t)(t2 / DEN);
    return floor1 - floor2;
}
