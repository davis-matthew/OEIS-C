/*
 * A328400  Smallest number with the same set of distinct prime exponents as
 *          n.
 * https://oeis.org/A328400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8252, u32 n <= 8252, u64 n <= 8252,
 *            u128 n <= 8252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A328400:
        a(n) = the smallest integer whose distinct prime exponents
        (ignoring multiplicities in n) are the same as those of n.
*/
#include <stddef.h>
#include <stdbool.h>

/*  First 50 prime numbers – more than enough for all exponents that can
    occur when n < 64‑bit.  The array can be enlarged if necessary.  */
static const unsigned long long FIRST_PRIMES[50] = {
    2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull,
    31ull, 37ull, 41ull, 43ull, 47ull, 53ull, 59ull, 61ull, 67ull, 71ull,
    73ull, 79ull, 83ull, 89ull, 97ull,101ull,103ull,107ull,109ull,113ull,
    127ull,131ull,137ull,139ull,149ull,151ull,157ull,163ull,167ull,173ull,
    179ull,181ull,191ull,193ull,197ull,199ull,211ull,223ull,227ull,229ull
};

seq_t a(idx_t n)
{
    /*  n = 1 ==> a(1) = 1  */
    if (n <= 1)
        return (seq_t)1;

    /*  Factor n: collect all prime exponents.  */
    idx_t tmp = n;
    idx_t exponents[64];
    size_t excnt = 0;

    /*  Trial division – sufficiently fast for n up to ~10⁶.  */
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            exponents[excnt++] = e;
        }
    }
    if (tmp > 1)      /* remaining prime factor contributes exponent 1 */
        exponents[excnt++] = 1;

    /*  Keep only distinct exponents.  */
    idx_t uniq[64];
    size_t uniqcnt = 0;
    for (size_t i = 0; i < excnt; ++i) {
        bool dup = false;
        for (size_t j = 0; j < uniqcnt; ++j)
            if (uniq[j] == exponents[i]) { dup = true; break; }
        if (!dup) uniq[uniqcnt++] = exponents[i];
    }

    /*  Sort distinct exponents in descending order (largest first).  */
    for (size_t i = 0; i < uniqcnt; ++i)
        for (size_t j = i + 1; j < uniqcnt; ++j)
            if (uniq[j] > uniq[i]) {
                idx_t t = uniq[i];
                uniq[i] = uniq[j];
                uniq[j] = t;
            }

    /*  Build the minimal number: assign largest exponents to the
        smallest primes.  */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < uniqcnt; ++i) {
        seq_t prime = (seq_t)FIRST_PRIMES[i];
        seq_t powv  = (seq_t)1;
        for (idx_t k = 0; k < uniq[i]; ++k)
            powv *= prime;              /* exponentiation by repeated multiplication */
        result *= powv;
    }
    return result;
}
