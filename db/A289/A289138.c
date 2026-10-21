/*
 * A289138  a(n) = smallest expomorphic number in base n: least integer k such
 *          that n^k ends in k, or 0 if no such k exists.
 * https://oeis.org/A289138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_len, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: For each base n, test the (only known) 11 positive
 * expomorphic candidates.  Return the smallest one that satisfies
 * n^k ≡ k (mod 10^digits(k)).  If none, return 0. */

#include "../utils/digit_len.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* known positive expomorphic numbers */
    static const seq_t cand[11] = {1, 3, 5, 6, 7, 9, 16, 36, 56, 76, 96};

    for (idx_t i = 0; i < 11; ++i)
    {
        seq_t k         = cand[i];
        int   d         = digit_len(k);                /* number of decimal digits of k  */
        unsigned int mod_int = (d == 1) ? 10U : 100U;   /* 10^d, fits in < 2^32 */

        /* compute n^k (mod 10^d) using the helper mod_pow */
        seq_t base     = (seq_t)(n % (uint64_t)mod_int);
        seq_t modulo   = (seq_t)mod_int;
        seq_t r        = mod_pow(base, (uint64_t)k, modulo);

        if (r == k)
            return k;   /* smallest candidate found */
    }
    return 0;   /* no expomorphic number for this base */
}
