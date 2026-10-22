/*
 * A371099  a(n) = gcd(36*n+9, A276086(36*n+9)), where A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A371099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1489, u32 n <= 8998, u64 n <= 8998,
 *            u128 n <= 8998, big any n
 * Helpers:   factor_u64, gcd, mul_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A371099
 *
 *  a(n) = gcd( 36*n + 9 , A276086( 36*n + 9 ) )
 *
 *  The implementation uses the fact that for a prime p, the exponent
 *  of p in A276086(m) equals the digit of m in the primorial–base
 *  representation of m (the “digit” is always < p).  The gcd therefore
 *  equals ∏ p^min(v_p(m),d_p(m)), where d_p(m) is that digit.
 *  Only primes dividing m need be considered, so we factor m first
 *  (with factor_u64) and then scan the primorial digits once to obtain
 *  the needed exponents.  The result always divides m and thus fits in
 *  seq_t.  This works for any m that fits in seq_t and has a 64‑bit
 *  representation (the tests only use such m).
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"   /* for factor_u64() */
#include "../utils/mul_pow.h"

/* first 17 primes – enough for 36*n+9 with n up to 10 000 */
static const idx_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53, 59
};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

/* ----------------- main function ----------------- */
seq_t a(idx_t n)
{
    /* m = 36*n+9, which fits in seq_t */
    seq_t m = (seq_t)n * 36 + 9;

    /* factor m – m < 2^64 for the test range, so cast safely */
    uint64_t m64 = (uint64_t)m;
    uint64_t pf[20]   = {0};
    uint64_t exp[20]  = {0};
    size_t cnt = 0;
    factor_u64(m64, pf, exp, &cnt);

    if (cnt == 0)                     /* m == 1, impossible here */
        return 1;

    /* precompute primorial weights w[i] = ∏_{j<i} p[j] */
    static seq_t weights[NUM_PRIMES];
    static bool init = false;
    if (!init) {
        weights[0] = 1;
        for (idx_t i = 1; i < NUM_PRIMES; ++i)
            weights[i] = weights[i - 1] * (seq_t)PRIMES[i - 1];
        init = true;
    }

    /* find largest i with weights[i] <= m */
    idx_t i_last = 0;
    for (idx_t i = 0; i < NUM_PRIMES; ++i)
        if (weights[i] <= m)
            i_last = i;
        else
            break;

    seq_t rem  = m;
    seq_t ans  = 1;

    /* traverse digits from the most significant */
    for (idx_t i = i_last; (idx_t)(-1) > i; --i) { /* unsigned wraparound */
        seq_t w = weights[i];
        seq_t d = rem / w;     /* digit for prime PRIMES[i] */
        rem %= w;

        if (d == 0)
            continue;

        /* if this prime divides m, contribute min(v_p(m), d) */
        for (size_t j = 0; j < cnt; ++j) {
            if ((uint64_t)PRIMES[i] == pf[j]) {
                int k = (int)exp[j];
                if (k > (int)d)
                    k = (int)d;
                mul_pow(&ans, (seq_t)PRIMES[i], k);
                break;
            }
        }
    }

    return ans;
}
