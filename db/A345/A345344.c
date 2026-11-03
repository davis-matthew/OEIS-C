/*
 * A345344  a(n) = Sum_{d^2|n} Omega(n/d^2).
 * https://oeis.org/A345344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d^2|n} Omega(n/d^2)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Factor n into primes[] and exps[], returns the number of prime factors in *cnt.
   The function is a simple trial‑division routine adequate for n up to at least 1e6
   (and considerably larger) and does not rely on external helpers. */
static void simple_factor(uint64_t n, uint64_t *primes, uint64_t *exponents, size_t *cnt)
{
    size_t c = 0;
    uint64_t tmp = n;
    if (tmp == 0) {
        *cnt = 0;
        return;
    }
    /* handle factor 2 */
    if ((tmp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            e++;
        }
        primes[c] = 2ULL;
        exponents[c] = e;
        c++;
    }
    /* handle odd factors */
    for (uint64_t p = 3ULL; p <= tmp / p; p += 2ULL) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            primes[c] = p;
            exponents[c] = e;
            c++;
        }
    }
    /* remaining prime */
    if (tmp > 1ULL) {
        primes[c] = tmp;
        exponents[c] = 1ULL;
        c++;
    }
    *cnt = c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but never used due to OEIS offset */

    /* 64‑bit indexing accumulators */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t pf_cnt = 0;

    /* factorise n */
    simple_factor((uint64_t)n, primes, exponents, &pf_cnt);

    /* compute total exponent sum and the maximum exponents for d^2|n */
    seq_t total_exp = 0;          /* sum of exponents of n */
    seq_t N = 1;                  /* number of admissible d (product of (maxE+1)) */
    uint64_t max_exp[64];

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t e = exponents[i];
        total_exp += (seq_t)e;
        uint64_t me = e / 2;          /* maximal exponent allowed in d for prime i */
        max_exp[i] = me;
        N *= (seq_t)(me + 1ULL);
    }

    /* sum of exponents chosen in d over all admissible d */
    seq_t sum_exp_sel = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t me = (seq_t)max_exp[i];
        seq_t part_sum_e = me * (me + 1ULL) / 2ULL;          /* sum_{e=0..me} e */
        seq_t factor = N / (me + 1ULL);                     /* how many times each e occurs */
        sum_exp_sel += factor * part_sum_e;
    }

    /* a(n) = N * total_exp - 2 * sum_exp_sel */
    seq_t result = N * total_exp - 2 * sum_exp_sel;
    return result;
}
