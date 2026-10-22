/*
 * A379470  For n >= 1, a(n) = Sum_{i = 0..(n - 1)} n / GCD(n, i!).
 * https://oeis.org/A379470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i = 0..(n - 1)} n / GCD(n, i!)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1 */

    /* ------------------------------------------------------------
       Factor n into its prime powers.
       primes[]  – distinct prime factors of n
       exps[]    – corresponding exponents
       cnt       – number of distinct prime factors
     ------------------------------------------------------------ */
    uint64_t nn = n;
    uint64_t primes[64]   = {0};
    unsigned  exps[64]    = {0};
    unsigned  cnt         = 0;

    /* factor 2 */
    if ((nn & 1ULL) == 0ULL) {
        unsigned e = 0;
        while ((nn & 1ULL) == 0ULL) {
            nn >>= 1;
            ++e;
        }
        primes[cnt] = 2ULL;
        exps[cnt++] = e;
    }

    /* factor odd primes */
    for (uint64_t d = 3ULL; d * d <= nn; d += 2ULL) {
        if (nn % d == 0ULL) {
            unsigned e = 0;
            while (nn % d == 0ULL) {
                nn /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt++] = e;
        }
    }
    if (nn > 1ULL) {
        primes[cnt] = nn;
        exps[cnt]   = 1U;
        ++cnt;
    }

    /* vp[j] – current exponent of primes[j] in (i!)   */
    unsigned vp[64] = {0};

    seq_t sum = 0;

    for (idx_t i = 0; i < n; ++i) {
        /* compute term = n / GCD(n, i!) = product p^(max(0, e - vp)) */
        seq_t term = 1;
        for (unsigned j = 0; j < cnt; ++j) {
            unsigned rem = exps[j] > vp[j] ? exps[j] - vp[j] : 0U;
            if (rem) {
                /* multiply by primes[j] ^ rem  */
                for (unsigned k = 0; k < rem; ++k) {
                    term *= (seq_t)primes[j];
                }
            }
        }

        sum += term;

        /* update vp for the next factorial (i+1)!   */
        uint64_t val = (uint64_t)(i + 1);
        for (unsigned j = 0; j < cnt; ++j) {
            uint64_t p = primes[j];
            unsigned  c = 0;
            while (val % p == 0ULL) {
                ++c;
                val /= p;
            }
            vp[j] += c;
            if (val == 1ULL) break;  /* no more prime factors */
        }
    }

    return sum;
}
