/*
 * A305724  Crystal ball sequence for the lattice C_10.
 * https://oeis.org/A305724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..10} binomial(20, 2k) * binomial(n + k, 10)
 *            (OEIS A305724 – Crystal ball sequence for the lattice C_10)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 101, u128 n <= 8653,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Pre‑computed binomial(20, 2k) for k = 0..10 */
    static const seq_t coef[11] = {
        1ULL,      /* C(20,0)   */
        190ULL,    /* C(20,2)   */
        4845ULL,   /* C(20,4)   */
        38760ULL,  /* C(20,6)   */
        125970ULL, /* C(20,8)   */
        184756ULL, /* C(20,10)  */
        125970ULL, /* C(20,12)  */
        38760ULL,  /* C(20,14)  */
        4845ULL,   /* C(20,16)  */
        190ULL,    /* C(20,18)  */
        1ULL       /* C(20,20)  */
    };

    seq_t sum = 0;
    for (idx_t k = 0; k <= 10; ++k)
    {
        seq_t term = binomial(n + k, 10);
        sum += coef[k] * term;
    }
    return sum;
}
