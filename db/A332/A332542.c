/*
 * A332542  a(n) is the smallest k such that n+(n+1)+(n+2)+...+(n+k) is
 *          divisible by n+k+1.
 * https://oeis.org/A332542
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that sum_{i=n}^{n+k} i is divisible by
 *            (n+k+1)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: enumerate divisors m of n*(n-1) (where m = n+k+1) and test the condition. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static void gen_divs_recursive(const uint64_t *primes, const uint32_t *exps, size_t pcnt,
                               int idx, seq_t cur, seq_t *divs, size_t *divcnt)
{
    if (idx == (int)pcnt) {
        divs[(*divcnt)++] = cur;
        return;
    }
    seq_t mult = 1;
    for (uint32_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_recursive(primes, exps, pcnt, idx + 1, cur * mult, divs, divcnt);
        mult *= (seq_t)primes[idx];
    }
}

static int cmp_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* behaviour for n < 3 is irrelevant */

    uint64_t nn = (uint64_t)n;
    uint64_t prod = nn * (nn - 1);           /* n*(n-1) fits in 64‑bit for n up to ~1e9 */

    /* factor prod */
    uint64_t primes[64];
    uint32_t exps[64];
    size_t pcnt = 0;
    uint64_t tmp = prod;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* generate all divisors of prod */
    static seq_t divs[2048];
    static size_t divcnt = 0;
    divcnt = 0;
    gen_divs_recursive(primes, exps, pcnt, 0, 1, divs, &divcnt);

    /* sort divisors */
    qsort(divs, divcnt, sizeof(seq_t), cmp_seq_t);

    /* test each divisor m > n to find the smallest satisfying the condition */
    for (size_t i = 0; i < divcnt; ++i) {
        seq_t m = divs[i];
        if (m <= (seq_t)n) continue;                /* we need m = n + k + 1 > n + 1 */
        seq_t S = ((m - (seq_t)n) * ((seq_t)n + m - 1)) / 2;
        if (S % m == 0) {                          /* divisibility condition met */
            return m - (seq_t)n - 1;               /* k = m - n - 1 */
        }
    }

    return 0;    /* should never reach here */
}
