/*
 * A331110  The sum of dual-Zeckendorf-infinitary divisors of n = Product_{i}
 *          p(i)^r(i): divisors d = Product_{i} p(i)^s(i), such that the dual
 *          Zeckendorf expansion (A104326) of each s(i) contains only terms
 *          that are in the dual Zeckendorf expansion of r(i).
 * https://oeis.org/A331110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e||n} Product_{t in Z(e)} (p^t + 1) where Z(e)
 *            is the dual-Zeckendorf representation of the exponent e. The
 *            representation of e is the subset of Fibonacci numbers
 *            (1,2,3,5,8,…) that sums to e and has the smallest possible
 *            maximum element (ties broken by taking the larger number...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined but keep harmless */
    /* ----- Precompute Fibonacci numbers and minimal-max representations ----- */
    static bool init = false;
    /* maximum exponent we expect to handle (log2 of maximum n). */
    #define MAX_E 64
    /* generate Fibonacci numbers up to MAX_E */
    static unsigned fib[20];
    static int fib_cnt = 0;
    static unsigned rep_mask[MAX_E + 1];
    static unsigned rep_max[MAX_E + 1];
    static short   rep_len [MAX_E + 1];
    if (!init) {
        fib[0] = 1; fib[1] = 2;
        fib_cnt = 2;
        while (fib_cnt < 20) {
            unsigned next = fib[fib_cnt-1] + fib[fib_cnt-2];
            if (next > MAX_E) break;
            fib[fib_cnt++] = next;
        }
        /* For each possible sum up to MAX_E, find the best subset */
        for (int e = 0; e <= MAX_E; ++e) {
            rep_mask[e] = 0;
            rep_max[e]  = UINT_MAX;
            rep_len[e]  = 0;
        }
        int total_masks = 1 << fib_cnt;
        for (int mask = 1; mask < total_masks; ++mask) {
            int sum  = 0;
            int mx   = 0;
            int count = 0;
            for (int j = 0; j < fib_cnt; ++j) if (mask & (1 << j)) {
                sum  += fib[j];
                if (fib[j] > mx) mx = fib[j];
                ++count;
            }
            if (sum > MAX_E) continue;
            if (mx < rep_max[sum] || (mx == rep_max[sum] && count > rep_len[sum])) {
                rep_mask[sum] = mask;
                rep_max[sum]  = mx;
                rep_len[sum]  = count;
            }
        }
        init = true;
    }

    /* ----- Factor n ----- */
    uint64_t primes[64];
    uint64_t exps [64];
    size_t  pcnt  = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        /* find representation of e */
        unsigned mask;
        unsigned mx;
        short   len;
        /* if e is small we use precomputed */
        if (e <= (uint64_t)MAX_E) {
            mask = rep_mask[(int)e];
            mx   = rep_max[(int)e];
            len  = rep_len [(int)e];
        } else {
            /* for large exponents recompute on the fly (rare) */
            /* build fibonacci list up to e */
            unsigned f[64];
            int fc = 0;
            f[fc++] = 1; f[fc++] = 2;
            while (fc < 64) {
                unsigned next = f[fc-1] + f[fc-2];
                if (next > e) break;
                f[fc++] = next;
            }
            int bc = 1 << fc;
            mask = 0;
            mx   = UINT_MAX;
            len  = 0;
            for (int m = 1; m < bc; ++m) {
                long long sum = 0, maxv = 0;
                int cnt = 0;
                for (int j = 0; j < fc; ++j) if (m & (1 << j)) {
                    sum  += f[j];
                    if (f[j] > maxv) maxv = f[j];
                    ++cnt;
                }
                if (sum > (long long)e) continue;
                if (sum == (long long)e) {
                    if (maxv < mx || (maxv == mx && cnt > len)) {
                        mask = m;
                        mx   = (unsigned)maxv;
                        len  = cnt;
                    }
                }
            }
        }

        seq_t prod = (seq_t)1;
        for (unsigned j = 0; j <= mx; ++j) {
            if (mask & (1U << j)) {
                /* fib numbers are stored in f array; we need a mapping */
                /* Since fib array corresponds to indices 0..cnt-1 */
                /* For precomputed part (e <= MAX_E) the fib list is known as fib[j] */
                unsigned term;
                if (e <= (uint64_t)MAX_E)
                    term = fib[j];
                else {
                    /* Compute fib on the fly for large e */
                    unsigned fv[64];
                    fv[0] = 1; fv[1] = 2;
                    for (int t = 2; t <= j; ++t) fv[t] = fv[t-1] + fv[t-2];
                    term = fv[j];
                }
                seq_t pt = big_pow((seq_t)p, (seq_t)term);
                prod *= (pt + (seq_t)1);
            }
        }
        result *= prod;
    }

    return result;
}
