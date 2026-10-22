/*
 * A346102  a(n) = A276086(A328622(n)).
 * https://oeis.org/A346102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276086(A328622(n)) * * The sequence is produced as
 *            follows: * * 1. Let d = n mod 6. The 2–3 part of a(n) is * 2^(d
 *            mod 2) * 3^e3 with * e3 = 0 for d∈{0,1}, 2 for d∈{2,3}, 1 for
 *            d∈{4,5}. * * 2. Let k = n / 6 (floor). Write k in base 5. For
 *            each base‑5 digit * (least‑significant first) prod...
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* part from the 2–3 pattern */
    int d = n % 6;
    int exp2 = d & 1;              /* exponent for 2 */
    int q = d / 2;
    int exp3;                      /* exponent for 3 */
    if (q == 0)      exp3 = 0;
    else if (q == 1) exp3 = 2;
    else             exp3 = 1;     /* q == 2 */

    seq_t base = 1;
    if (exp2) base *= 2;
    if (exp3 == 1) base *= 3;
    else if (exp3 == 2) base *= 9;   /* 3*3 */

    /* part from the block multiplier */
    idx_t k = n / 6;
    seq_t mul = 1;

    /* first several primes >=5 – more than enough for all probable inputs */
    static const unsigned primes[] = {
        5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
        67,71,73,79,83,89,97,101,103,107,109,113,127,131,
        137,139,149,151,157,163,167,173,179,181,191,193,
        197,199,211,223,227,229,233,239,241,251,257,263,
        269,271,277,281,283,293,307,311,313,317,331,337,
        347,349,353,359,367,373,379,383,389,397,401,409,
        419,421,431,433,439,443,449,457,461,463,467,479,
        487,491,499,503,509,521,523,541
    };
    size_t pi = 0;   /* index into primes array */

    while (k > 0) {
        unsigned digit = k % 5;
        unsigned e = (2 * digit) % 5;
        if (e) {
            seq_t p = primes[pi];
            for (unsigned i = 0; i < e; ++i)
                mul *= p;
        }
        k /= 5;
        ++pi;
    }

    return base * mul;
}
