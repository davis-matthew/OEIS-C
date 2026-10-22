/*
 * A353487  a(n) = A276086(2*n) mod 4, where A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A353487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276086(2*n) mod 4 * Computed as the parity of the
 *            exponents of primes ≡3 (mod 4) * in the primorial‑base expansion
 *            of 2*n. Return 1 when the parity * is even, otherwise 3
 * Range:     u8 n <= 104, u16 n <= 9718, u32 n <= 9718, u64 n <= 9718,
 *            u128 n <= 9718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* small, fixed list of primes.  Only the first 25 are needed for
 * n up to 2^64 (m = 2*n) because the primorials grow extremely fast. */
static const idx_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL,
    67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t _NUM_PRIMES = sizeof(_primes)/sizeof(_primes[0]);

seq_t a(idx_t n)
{
    /* m = 2*n   (use seq_t to avoid overflow) */
    seq_t m = (seq_t)n * (seq_t)2;
    if (m == (seq_t)0)            /* only for n==0 */
        return (seq_t)1;

    /* primorials[i] = Π_{k=0}^{i-1} p_k   (primorial of i primes)   */
    seq_t primorials[_NUM_PRIMES];
    primorials[0] = (seq_t)1;                 /* P_0 = 1 */
    size_t last = 0;                          /* last index with primorial <= m */

    while (primorials[last] <= m) {
        if (last + 1 >= _NUM_PRIMES)          /* should never happen for n<2^64 */
            break;
        ++last;
        primorials[last] = primorials[last-1] * (seq_t)_primes[last-1];
    }

    /* Compute the digits of m in the primorial base, from the highest
     * exponent downwards, accumulating the parity of exponents of
     * primes ≡3 (mod 4). */
    int parity = 0;                            /* 0=even, 1=odd */
    for (size_t i = last; i > 0; --i) {        /* i-1 corresponds to prime index */
        seq_t digit = m / primorials[i-1];
        m = m % primorials[i-1];
        if (_primes[i-1] % 4ULL == 3ULL)
            parity ^= (int)(digit & (seq_t)1);
    }

    return parity ? (seq_t)3 : (seq_t)1;
}
