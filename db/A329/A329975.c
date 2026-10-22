/*
 * A329975  Beatty sequence for 1 + x + x^2, where x is the real solution of
 *          1/x + 1/(1+x+x^2) = 1.
 * https://oeis.org/A329975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1+x+x^2)), where x is the real solution of x^3 -
 *            x - 1 = 0 (the plastic constant). An equivalent exact
 *            representation is α = 1 + x + x^2 ≈ 4.07959562349143814926.
 *            Using a fixed‑point approximation with SCALE = 10^18 we compute
 *            a(n) = floor( (n * α_num) / SCALE ) where α_num = roun...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed‑point scaling constant (10^18).  For 128‑bit builds
       we use unsigned __int128 for the intermediate results. */
    const unsigned __int128 SCALE   = (unsigned __int128)1000000000000000000ULL;
    const unsigned __int128 ALPHA_N = (unsigned __int128)4079595623491438149ULL;

    /* Compute floor(n * α) with exact integer arithmetic. */
    unsigned __int128 prod = (unsigned __int128)n * ALPHA_N;
    return (seq_t)(prod / SCALE);
}
