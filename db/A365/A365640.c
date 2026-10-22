/*
 * A365640  Prime powers of the form 2^k + 1.
 * https://oeis.org/A365640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime power of the form 2^k+1 (the Fermat primes together
 *            with 2 and 9)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Known terms: 2, 3, 5, 9, 17, 257, 65537.
   No further terms are known, so for n beyond these we return 0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static array of all known terms. */
    static const seq_t terms[] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)9,
                                   (seq_t)17, (seq_t)257, (seq_t)65537 };
    const size_t count = sizeof terms / sizeof terms[0];

    if (n < (idx_t)1)
        return (seq_t)0;            /* undefined for n < 1 */

    idx_t idx = n - 1;
    if (idx < (idx_t)count)
        return terms[idx];

    /* No further terms known. */
    return (seq_t)0;
}
