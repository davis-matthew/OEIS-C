/*
 * A360407  Irregular table T(n, k), n >= 0, k = 0..A002110(n)-1, read by
 *          rows; for any k with primorial base expansion (d_n, ..., d_1),
 *          T(n, k) is the least number t such that t mod prime(u) = d_u for u
 *          = 1..n (where prime(u) denotes the u-th prime number).
 * https://oeis.org/A360407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 2558, u32 n <= 2558, u64 n <= 2558,
 *            u128 n <= 2558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A360407 via primorial base representation
   and the Chinese remainder theorem.                                            */

seq_t a(idx_t n)
{
    /* First 15 primes are sufficient for all n up to 10000.  */
    static const uint32_t PRIMES[15] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47
    };

    idx_t r = 0;               /* row index */
    idx_t prim = 1;            /* product of first r primes (primorial) */
    idx_t cum = 0;             /* cumulative number of terms before row r */

    /* Find the row r such that cum + prim > n.  */
    while (cum + prim <= n) {
        cum += prim;
        prim *= (idx_t)PRIMES[r];
        ++r;
    }

    /* Column index within the found row.  */
    idx_t k = n - cum;

    idx_t res = 0;            /* current solution modulo 'mod' */
    idx_t mod = 1;            /* product of processed primes */
    idx_t tmpk = k;           /* remaining part of k while extracting digits */

    /* Incrementally solve the CRT system.  */
    for (idx_t i = 0; i < r; ++i) {
        idx_t p = PRIMES[i];
        idx_t d = tmpk % p;
        tmpk /= p;

        /* Find the smallest res ≡ d (mod p).  */
        while (res % p != d) {
            res += mod;
        }

        mod *= p;
    }

    return (seq_t)res;
}
