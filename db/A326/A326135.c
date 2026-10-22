/*
 * A326135  a(n) = sigma(A028234(n)), where sigma is the sum of divisors of n,
 *          and A028234 gives n without any occurrence of its smallest prime
 *          factor.
 * https://oeis.org/A326135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A028234(n))
 * Range:     u8 n <= 329, u16 n <= 9530, u32 n <= 9530, u64 n <= 9530,
 *            u128 n <= 9530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A028234(n) = n with all occurrences of its smallest prime factor removed */

#include <stdint.h>
#include <stddef.h>

/* Integer square‑root of a 64‑bit unsigned integer.        */
/* Returns the greatest r such that r*r <= x.               */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (x > 0xFFFFFFFFULL ? 0xFFFFFFFFULL : x);
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = ( __uint128_t)mid * mid;
        if (sq == x)
            return mid;
        else if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Computes the sum of divisors of m (m >= 1).               */
static seq_t sigma_of_uint64(uint64_t m)
{
    seq_t result = 1;

    /* handle factor 2 separately */
    if ((m & 1) == 0) {
        int exp = 0;
        while ((m & 1) == 0) { m >>= 1; exp++; }
        seq_t term = 1;
        /* 2^(exp+1) - 1   */
        for (int k = 0; k < exp + 1; ++k) term *= (seq_t)2ULL;
        term -= 1;
        result *= term;
    }

    uint64_t i = 3;
    uint64_t lim = isqrt_u64(m);
    while (i <= lim) {
        if (m % i == 0) {
            int exp = 0;
            while (m % i == 0) { m /= i; exp++; }
            seq_t p_pow = 1;
            seq_t p_seq = (seq_t)i;
            for (int k = 0; k < exp + 1; ++k) p_pow *= p_seq;     /* p^(exp+1) */
            seq_t term = (p_pow - 1) / (p_seq - 1);               /* (p^(exp+1)-1)/(p-1) */
            result *= term;
            lim = isqrt_u64(m);
        }
        i += 2;
    }

    if (m > 1) {                     /* remaining prime factor > sqrt */
        seq_t p_seq = (seq_t)m;
        result *= (p_seq + 1ULL);     /* p^1 contributes p + 1 */
    }

    return result;
}

/* Main sequence function.                                   */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1                                   */
    if (n == 0) return 0;
    if (n == 1) return 1;                                    /* A028234(1) = 1 */

    uint64_t nn = (uint64_t)n;
    uint64_t spf;

    /* --- find smallest prime factor ------------------------------------ */
    if ((nn & 1) == 0) {
        spf = 2;
    } else {
        spf = 0;
        uint64_t lim = isqrt_u64(nn);
        for (uint64_t d = 3; d <= lim; d += 2) {
            if (nn % d == 0) { spf = d; break; }
        }
        if (spf == 0) spf = nn;            /* nn itself is prime       */
    }

    /* --- remove all occurrences of spf --------------------------------- */
    uint64_t m = nn;
    while (m % spf == 0) m /= spf;          /* m now has no factor spf  */

    if (m <= 1) return 1;                   /* sigma(1) = 1             */

    /* --- compute sigma(m) --------------------------------------------- */
    return sigma_of_uint64(m);
}
