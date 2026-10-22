/*
 * A300728  a(n) is the smallest positive number k such that k^2 + k*n + n^2
 *          is a perfect square, or 0 if no such k exists.
 * https://oeis.org/A300728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300728:  a(n) = smallest positive integer k such that
   k^2 + k*n + n^2 is a perfect square; 0 if no such k exists. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool sieve_done = false;
static unsigned *primes = NULL;
static size_t prime_cnt = 0;

/* simple sieve up to 20000 – enough for all n ≤ 10000 */
static void init_primes(void)
{
    if (sieve_done) return;
    const int LIM = 20000;
    bool *is_composite = (bool*)calloc(LIM + 1, 1);
    size_t allocated = LIM / 2;
    primes = (unsigned*)malloc(allocated * sizeof(unsigned));
    size_t pos = 0;

    for (int i = 2; i <= LIM; ++i) {
        if (!is_composite[i]) {
            if (pos == allocated) {
                allocated *= 2;
                primes = (unsigned*)realloc(primes, allocated * sizeof(unsigned));
            }
            primes[pos++] = (unsigned)i;
            if ((int64_t)i * i <= LIM)
                for (int j = i * i; j <= LIM; j += i)
                    is_composite[j] = true;
        }
    }
    prime_cnt = pos;
    free(is_composite);
    sieve_done = true;
}

static void factor_uint64(uint64_t n, unsigned *out_primes, int *out_exps, size_t *out_cnt)
{
    *out_cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        unsigned p = primes[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            out_primes[(*out_cnt)] = p;
            out_exps[(*out_cnt)] = e;
            ++(*out_cnt);
        }
    }
    if (n > 1) {
        out_primes[(*out_cnt)] = (unsigned)n;
        out_exps[(*out_cnt)] = 1;
        ++(*out_cnt);
    }
}

static void generate_all_divisors(const unsigned *primes, const int *exps,
                                  size_t nf, uint64_t *divs, size_t *cnt)
{
    divs[0] = 1;
    *cnt = 1;
    for (size_t i = 0; i < nf; ++i) {
        uint64_t p = (uint64_t)primes[i];
        int e = exps[i];
        size_t curcnt = *cnt;
        uint64_t pe = 1;
        for (int k = 1; k <= e; ++k) {
            pe *= p;
            for (size_t j = 0; j < curcnt; ++j) {
                divs[(*cnt)] = divs[j] * pe;
                ++(*cnt);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) return (seq_t)1;

    init_primes();

    /* Compute M = 3*n*n.  It fits in 64 bits for n ≤ 10000. */
    uint64_t nn = (uint64_t)n;
    uint64_t M  = 3ULL * nn * nn;

    /* Factor M */
    unsigned fpr[32];
    int fex[32];
    size_t fcnt = 0;
    factor_uint64(M, fpr, fex, &fcnt);

    /* Generate all divisors of M */
    uint64_t divs[2048];
    size_t dcnt = 0;
    generate_all_divisors(fpr, fex, fcnt, divs, &dcnt);

    seq_t best = 0;  /* 0 means unset yet */

    for (size_t idx = 0; idx < dcnt; ++idx) {
        uint64_t u = divs[idx];
        uint64_t v = M / u;
        if (u > v) continue;                 /* ensure u <= v  */

        /* u and v must have same parity for integers x,y */
        if ((u & 1) != (v & 1)) continue;

        /* Compute k = (v - u - 2*n) / 4  */
        uint64_t numer = v - u - 2ULL * nn;
        if (numer <= 0) continue;
        if (numer % 4 != 0) continue;

        uint64_t k = numer / 4;
        if (k == 0) continue;                /* k must be positive */

        if (best == 0 || k < best) best = (seq_t)k;
    }

    return best;   /* 0 if none found */
}
