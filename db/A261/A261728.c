/*
 * A261728  a(1)=1; a(2*n) = 3*n; for odd n>1, a(n) is the smallest number not
 *          already present which is entailed by the rules (i) k present =>
 *          3*k+1 present; (ii) 2*k present => k present.
 * https://oeis.org/A261728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261728
 * Closed‑form: 
 *   if n even       -> a(n) = 3 * (n / 2)  
 *   if n ≡ 1 (mod 6) -> a(n) = n
 *   if n ≡ 3 (mod 6) -> a(n) = n + 1
 *   if n ≡ 5 (mod 6) -> a(n) = (n – 1) / 2
 * (All indices start at 1.)*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1ULL) == 0)
        return (seq_t)(n / 2) * 3;

    /* odd indices – use residue mod 6 */
    switch (n % 6)
    {
        case 1:   return (seq_t)n;
        case 3:   return (seq_t)(n + 1);
        case 5:   return (seq_t)((n - 1) / 2);
        default:  /* unreachable for odd n */
            return 0;
    }
}
