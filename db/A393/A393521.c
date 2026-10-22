/*
 * A393521  Number of minimum total dominating sets in the n-necklace graph.
 * https://oeis.org/A393521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*k) = 4^(k+1); a(4*k+1) = 6*(4*k+1)*4^k; a(4*k+2) =
 *            (2*k+1)*(61*k+34)*4^k; a(4*k+3) = 10*(4*k+3)*4^k;
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 41, u64 n <= 97, u128 n <= 221,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 2;            /* k = floor(n/4) */
    seq_t pow4k = 1;             /* compute 4^k */
    for (idx_t i = 0; i < k; ++i)
        pow4k *= (seq_t)4;

    seq_t result;
    switch (n & 3) {
    case 0:  /* a(4*k) = 4^(k+1) = 4 * 4^k */
        result = pow4k * (seq_t)4;
        break;
    case 1:  /* a(4*k+1) = 6*(4*k+1)*4^k */
        result = ((seq_t)6 * ((seq_t)4 * (seq_t)k + (seq_t)1)) * pow4k;
        break;
    case 2:  /* a(4*k+2) = (2*k+1)*(61*k+34)*4^k */
        result = ((seq_t)2 * (seq_t)k + (seq_t)1) *
                 ((seq_t)61 * (seq_t)k + (seq_t)34) * pow4k;
        break;
    case 3:  /* a(4*k+3) = 10*(4*k+3)*4^k */
        result = ((seq_t)10 * ((seq_t)4 * (seq_t)k + (seq_t)3)) * pow4k;
        break;
    default:
        result = (seq_t)0; /* unreachable */
    }
    return result;
}
