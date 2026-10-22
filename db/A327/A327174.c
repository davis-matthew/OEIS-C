/*
 * A327174  a(n) = [(2*n+1)*r] - [(n+1)*r] - [n*r], where [ ] = floor and r =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A327174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+1)*phi) - floor((n+1)*phi) - floor(n*phi)
 * Range:     u8 n <= 56, u16 n <= 9322, u32 n <= 9322, u64 n <= 9322,
 *            u128 n <= 9322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = (1 + sqrt(5)) / 2 */

/* Precision constants for approximating sqrt(5):
 * sqrt(5) ≈ 2236067977499789696 / 1e18  (error < 0.5 / 1e18 )
 */
#define SCALE_SQ5     1000000000000000000ULL
#define NUM_SQ5       2236067977499789696ULL

/* Compute floor(k * sqrt(5)) using integer arithmetic */
static inline seq_t floor_sqrt5(idx_t k)
{
    __uint128_t prod = ( __uint128_t )k * NUM_SQ5;
    uint64_t val = (uint64_t)(prod / SCALE_SQ5);  /* value fits in 64 bits */
    return (seq_t)val;
}

/* Compute floor(k * phi) = floor( (k + k*sqrt(5)) / 2 ) */
static inline seq_t floor_phi(idx_t k)
{
    seq_t sqrt5_floor = floor_sqrt5(k);
    return ((seq_t)k + sqrt5_floor) / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t a2n1 = floor_phi(2 * n + 1);
    seq_t an1  = floor_phi(n + 1);
    seq_t an   = floor_phi(n);
    return a2n1 - an1 - an;
}
