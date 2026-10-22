/*
 * A313748  Coordination sequence Gal.5.226.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=5,a(2)=10,a(3)=15,a(4)=20,a(5)=26,a(6)=31,a(7)=37,
 *            a(8)=42,a(9)=48,a(10)=53,a(11)=58; for n≥12: a(n)=a(n-1)+{4 if
 *            n%3==2 else 6}
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Early terms */
    switch (n)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)10;
        case 3: return (seq_t)15;
        case 4: return (seq_t)20;
        case 5: return (seq_t)26;
        case 6: return (seq_t)31;
        case 7: return (seq_t)37;
        case 8: return (seq_t)42;
        case 9: return (seq_t)48;
        case 10: return (seq_t)53;
        case 11: return (seq_t)58;
        default: break;
    }

    /* For n ≥ 12 use the closed form */
    uint64_t sum = 58ULL;            /* a(11) */
    idx_t    k = n - 11;             /* number of steps from 11 to n   */
    idx_t    cycles = k / 3;
    idx_t    rem    = k % 3;

    sum += (uint64_t)cycles * 16ULL; /* each full cycle contributes 6+6+4 = 16 */

    if (rem == 1)          sum += 6ULL;
    else if (rem == 2)     sum += 12ULL;   /* (6+6) */

    return (seq_t)sum;
}
