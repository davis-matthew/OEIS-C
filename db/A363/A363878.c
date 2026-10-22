/*
 * A363878  Number of divisors of 7*n-4 of form 7*k+2.
 * https://oeis.org/A363878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-4) with d ≡ 2 (mod 7)
 * Range:     u8 n <= 39, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   count_mod7_divs, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/count_mod7_divs.h"

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 */
        return (seq_t)0;

    /* m = 7*n - 4 */
    seq_t m = ((seq_t)7 * (seq_t)n) - (seq_t)4;

    /* prime factorisation of m (u64 fast routine) */
    /* Max number of distinct prime factors for m <= 7*10000-4 < 70000
       is small (<10), so allocate a buffer of 32. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Count divisors with residue 2 (mod 7) */
    uint64_t result = 0;
    count_mod7_divs(primes, exps, cnt, 0, 1 % 7, &result);

    return (seq_t)result;
}
