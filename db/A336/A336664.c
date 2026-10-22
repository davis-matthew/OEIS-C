/*
 * A336664  Number of distinct divisors d of n with the property that d = (the
 *          number of nonnegative bases m < n such that m^k == m (mod n))/(the
 *          number of nonnegative bases m < n such that -m^k == m (mod n)) for
 *          some nonnegative k.
 * https://oeis.org/A336664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3755, u16 n <= 3755, u32 n <= 3755, u64 n <= 3755,
 *            u128 n <= 3755, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes a(n) by brute‑force enumeration of k
   up to 2·λ(n), where λ is the Carmichael function of n.  For each
   k we count the numbers m∈[0,n) that satisfy
        m^k ≡ m (mod n)   (denote this count by A)
   and
        -m^k ≡ m (mod n)  (denote this count by B).
   If A/B is an integer divisor of n we record it.  The return value
   is the number of distinct such divisors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* gcd for unsigned 64 bit integers */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0ULL)
    {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* obtain Carmichael function λ(n) for n ≥ 1 */
static uint64_t lambda_u64(uint64_t n)
{
    if (n <= 1) return 1ULL;
    uint64_t lam = 1ULL;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            uint64_t lam_p;
            if (p == 2)
            {
                if (exp == 1) lam_p = 1ULL;
                else if (exp == 2) lam_p = 2ULL;
                else lam_p = 1ULL << (exp - 2);
            }
            else
            {
                uint64_t pow_val = 1;
                for (int i = 0; i < exp - 1; ++i) pow_val *= p;
                lam_p = (p - 1) * pow_val;
            }
            uint64_t g = gcd_u64(lam, lam_p);
            lam = (lam / g) * lam_p;
        }
    }
    if (temp > 1)
    {
        uint64_t p = temp;
        uint64_t lam_p = (p == 2) ? 1ULL : (p - 1);
        uint64_t g = gcd_u64(lam, lam_p);
        lam = (lam / g) * lam_p;
    }
    return lam;
}

seq_t a(idx_t n)
{
    /* handle trivial case n==1 */
    if (n == 0) return 0;  /* undefined, but to keep type safety */

    /* allocate power array and found-divisor mask */
    uint64_t *pow = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    uint8_t  *found = (uint8_t  *)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!pow || !found) return 0;   /* allocation failure fallback */

    /* initialise m^0 = 1 mod n for all m in [0,n) */
    uint64_t init = (n == 1) ? 0 : 1 % n;
    for (size_t i = 0; i < (size_t)n; ++i) pow[i] = init;

    /* upper bound for k: 2 * λ(n) */
    uint64_t lam = lambda_u64(n);
    uint64_t maxK = 2 * lam;   /* safe upper bound */

    for (uint64_t k = 0; k <= maxK; ++k)
    {
        uint64_t A = 0, B = 0;
        for (size_t m = 0; m < (size_t)n; ++m)
        {
            uint64_t val = pow[m];
            if (val == m) ++A;
            uint64_t neg = (n == 1) ? 0 : (n - val) % n;
            if (neg == m) ++B;
        }

        if (B > 0 && (A % B) == 0)
        {
            uint64_t ratio = A / B;
            if (ratio <= n && n % ratio == 0)
                found[ratio] = 1;
        }

        /* prepare next k: compute (m * current)^k+1 ≡ m^(k+1) */
        for (size_t m = 0; m < (size_t)n; ++m)
        {
            pow[m] = (pow[m] * m) % n;
        }
    }

    /* count distinct divisors that were found */
    uint64_t cnt = 0;
    for (size_t d = 1; d <= (size_t)n; ++d)
        if (found[d]) ++cnt;

    free(pow);
    free(found);
    return (seq_t)cnt;
}
