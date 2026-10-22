/*
 * A393359  a(n) is the number of ways to successively choose subsets of sizes
 *          equal to the proper divisors of n from [n], each choice taken from
 *          the remaining elements.
 * https://oeis.org/A393359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if σ(n) > 2n, else a(n) = Π_{i=1..τ(n)-1} binomial(n -
 *            Σ_{j< i} d_j , d_i), where d_1 > d_2 > … > d_{τ(n)-1} are the
 *            proper divisors of n
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 43, u128 n <= 74,
 *            big any n
 * Helpers:   binomial, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/sigma_of.h"

/* compare function for descending order of uint64_t values */
static int cmp_desc(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

/* recursively generate all divisors of n, storing them in dst; cur holds the product of chosen primes so far */
static void gen_divs(const uint64_t *primes, const int *exps, size_t pcnt,
                     size_t idx, uint64_t cur, uint64_t n,
                     uint64_t *dst, size_t *cnt)
{
    if (idx == pcnt) {
        if (cur != n) {
            dst[(*cnt)++] = cur;
        }
        return;
    }
    uint64_t v = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(primes, exps, pcnt, idx + 1, v, n, dst, cnt);
        v *= primes[idx];
    }
}

/* collect all proper divisors of n (excluding n itself) into an array */
static size_t proper_divisors(uint64_t n, uint64_t *dst)
{
    /* maximum number of prime factors for n in the required range */
    const size_t MAX_PRMS = 16;
    uint64_t primes[MAX_PRMS];
    int      exps  [MAX_PRMS];
    size_t   pcnt  = 0;

    uint64_t m = n;
    /* trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps [pcnt] = e;
            ++pcnt;
            if (pcnt == MAX_PRMS) break;
        }
    }
    if (m > 1 && pcnt < MAX_PRMS) {          /* remaining prime factor */
        primes[pcnt] = m;
        exps [pcnt] = 1;
        ++pcnt;
    }

    size_t cnt = 0;
    gen_divs(primes, exps, pcnt, 0, 1, n, dst, &cnt);

    /* sort divisors in descending order */
    qsort(dst, cnt, sizeof(uint64_t), cmp_desc);
    return cnt;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence starts at 1 */

    seq_t sigma = sigma_of(n);
    seq_t two_n = (seq_t)n * (seq_t)2;

    if (sigma > two_n) return 0;              /* abundant number */

    /* collect proper divisors */
    uint64_t divs[512];
    size_t div_cnt = proper_divisors((uint64_t)n, divs);

    seq_t result = 1;
    idx_t rem    = n;

    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = (idx_t)divs[i];
        seq_t bin = binomial(rem, d);
        result *= bin;
        rem -= d;
    }

    return result;
}
