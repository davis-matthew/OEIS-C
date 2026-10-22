/*
 * A400796  a(n) = (n^2 + 4*Legendre(n,5))/5.
 * https://oeis.org/A400796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = (n^2 + 4*Legendre(n,5))/5
 * Range:     u8 n <= 35, u16 n <= 572, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed‑form depending on n mod 5:          */
/*   n = 5k      -> a = 5k^2                     */
/*   n = 5k+1    -> a = 5k^2 + 2k + 1            */
/*   n = 5k+2    -> a = 5k^2 + 4k                */
/*   n = 5k+3    -> a = 5k^2 + 6k + 1            */
/*   n = 5k+4    -> a = 5k^2 + 8k + 4            */

seq_t a(idx_t n)
{
    /* work with the quotient and remainder of n/5 */
    idx_t k = n / 5;
    idx_t r = n % 5;

    seq_t K   = (seq_t)k;
    seq_t res = 0;

    /* evaluate the appropriate closed form */
    switch (r) {
        case 0:
            res = 5 * K * K;
            break;
        case 1:
            res = 5 * K * K + 2 * K + 1;
            break;
        case 2:
            res = 5 * K * K + 4 * K;
            break;
        case 3:
            res = 5 * K * K + 6 * K + 1;
            break;
        case 4:
            res = 5 * K * K + 8 * K + 4;
            break;
        default:
            /* unreachable – r ∈ {0,…,4} by construction */
            break;
    }
    return res;
}
