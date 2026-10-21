/*
 * A252942  Smallest prime of the form "Concatenate(m,n,m)".
 * https://oeis.org/A252942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime that equals concat(m, n, m) for some m ≥ 1
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 10, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    for (idx_t m = 1; ; ++m)
    {
        /* skip m whose last digit forces the candidate to be even or divisible by 5 */
        idx_t last = m % 10;
        if ((last & 1) == 0 || last == 5)
            continue;

        /* digit lengths of m and n */
        int dm = digit_len((seq_t)m);
        int dn = digit_len((seq_t)n);

        /* powers of 10 needed for the concatenation */
        seq_t pow_dm    = pow10((idx_t)dm);          /* 10^dm   */
        seq_t pow_dn_dm = pow10((idx_t)(dn + dm));   /* 10^(dn+dm) */

        /* candidate = m * 10^(dn+dm) + n * 10^dm + m */
        seq_t cand = ((seq_t)m) * pow_dn_dm
                   + ((seq_t)n) * pow_dm
                   + ((seq_t)m);

        if (is_prime_big(cand))
            return cand;
    }
}
