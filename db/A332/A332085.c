/*
 * A332085  Number of ordered pairs of divisors of n, (d1,d2), such that d1 is
 *          prime and d1 <= d2.
 * https://oeis.org/A332085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|n, d2|n, d1 is prime, d1 <= d2} 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    return (va > vb) - (va < vb);
}

/* recursive divisor generation */
static void gen_divs(const uint64_t *primes, const unsigned *exps, size_t idx, size_t cnt,
                     uint64_t current, uint64_t *divs, size_t *divc) {
    if (idx == cnt) {
        divs[(*divc)++] = current;
        return;
    }
    uint64_t val = current;
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, idx + 1, cnt, val, divs, divc);
        val *= primes[idx];
    }
}

/* external function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined, but safe guard */
    uint64_t nn = n;

    /* prime factorisation of n (trial division is enough for n <= 10000) */
    uint64_t primes[20];
    unsigned exps[20];
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            unsigned e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (nn > 1) {
        primes[cnt] = nn;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* generate all divisors of n */
    uint64_t divs[256];   /* 256 > max τ(n) for n <= 10000 */
    size_t divc = 0;
    gen_divs(primes, exps, 0, cnt, (uint64_t)1, divs, &divc);

    /* sort the divisor list */
    qsort(divs, divc, sizeof(uint64_t), cmp_uint64);

    /* accumulate the answer */
    seq_t ans = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        /* binary search for first divisor >= p */
        size_t lo = 0, hi = divc;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (divs[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        size_t num_ge = divc - lo;            /* number of divisors >= p */
        ans += (seq_t)num_ge;
    }
    return ans;
}
