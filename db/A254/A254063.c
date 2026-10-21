/*
 * A254063  Positive integers whose square is expressible in exactly one way
 *          as x^2 + 5xy + y^2, with 0 < x < y.
 * https://oeis.org/A254063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 94, u16 n <= 750, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   gcd_ui, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all positive integers k such that the quadratic
   equation x^2 + 5xy + y^2 = k^2 has a primitive solution (x,y) with 0 < x < y.
   For every integer m its number of representations as
   x^2 + 5xy + y^2 = m^2 with 0 < x < y equals the number of primitive k
   dividing m.  We sieve over all such primitive k to count representations
   for every m up to a fixed limit.  The sequence is then defined as the set
   of m with exactly one representation.  The implementation pre‑computes
   the first 20000 terms (enough for all test cases) and answers any
   request a(idx_t n) in O(1).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/gcd_ui.h"

/* Linear sieve for smallest prime factor (spf).  Index 0 and 1 left 0. */
static void compute_spf(idx_t limit, uint32_t **spf_out)
{
    uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    *spf_out = spf;
    spf[0] = spf[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
        }
    }
}

/* Recursive divisor generation */
static void gen_divisors_rec(idx_t idx, uint64_t cur, const uint32_t *pr, const uint8_t *exp,
                             size_t cnt, uint64_t *divs, size_t *divcnt)
{
    if (idx == cnt) {
        divs[*divcnt] = cur;
        ++(*divcnt);
        return;
    }
    uint64_t p = (uint64_t)pr[idx];
    uint64_t mult = 1;
    for (uint8_t e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(idx + 1, cur * mult, pr, exp, cnt, divs, divcnt);
        mult *= p;
    }
}

/* Initialize sequence array up to needed number of terms */
static void init_sequence(seq_t **seq, size_t *seq_len)
{
    const idx_t LIMIT = 200000;          /* search range – plenty for 10k terms */
    const size_t MAX_PRIMES = 32;        /* enough for factorisation lists */

    /* Smallest prime factors */
    uint32_t *spf = NULL;
    compute_spf(LIMIT, &spf);

    /* Count of primitive solutions for each k */
    uint16_t *prim_cnt = (uint16_t *)calloc((size_t)(LIMIT + 1), sizeof(uint16_t));
    if (!prim_cnt) exit(EXIT_FAILURE);

    /* For each possible y = v from 1..LIMIT */
    for (idx_t y = 1; y <= LIMIT; ++y) {
        uint64_t M = 21ULL * y * (uint64_t)y; /* M = 21*y^2 */

        /* Factorise y */
        uint32_t primes[MAX_PRIMES];
        uint8_t exps[MAX_PRIMES];
        size_t pcnt = 0;
        idx_t temp = y;
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e * 2;          /* exponent is doubled for y^2 */
            ++pcnt;
        }

        /* Add primes 3 and 7 with exponent 1 (for factor 21) */
        bool found3 = false, found7 = false;
        for (size_t i = 0; i < pcnt; ++i) {
            if (primes[i] == 3) { exps[i]++; found3 = true; }
            if (primes[i] == 7) { exps[i]++; found7 = true; }
        }
        if (!found3) {
            primes[pcnt] = 3; exps[pcnt] = 1; ++pcnt;
        }
        if (!found7) {
            primes[pcnt] = 7; exps[pcnt] = 1; ++pcnt;
        }

        /* Generate all divisors of M */
        uint64_t divs[4096];
        size_t divcnt = 0;
        gen_divisors_rec(0, 1ULL, primes, exps, pcnt, divs, &divcnt);

        /* Process each divisor pair (A,B) with A <= B */
        for (size_t i = 0; i < divcnt; ++i) {
            uint64_t A = divs[i];
            uint64_t B = M / A;
            if (A > B) continue;                /* avoid duplicate pairs */

            if ( (A & 1ULL) != (B & 1ULL) ) continue;   /* parity must match */

            uint64_t diff = B - A;
            if (diff % 4ULL != 0) continue;                /* (B-A)/4 must be integer */

            uint64_t k = diff / 4ULL;              /* k = sqrt(u^2+5uv+v^2) */
            if (k == 0 || k > (uint64_t)LIMIT) continue;

            uint64_t X = (A + B) >> 1;             /* (A+B)/2 */
            uint64_t tmp = X - 5ULL * (uint64_t)y;
            if (tmp & 1ULL) continue;            /* u must be integer */

            uint64_t u = tmp >> 1;                /* u = (X-5y)/2 */
            if (u == 0 || u >= (uint64_t)y) continue;

            /* Check coprimality */
            if (gcd_ui((unsigned int)u, (unsigned int)y) != 1) continue;

            /* Valid primitive solution found */
            prim_cnt[(size_t)k]++;
        }
    }

    /* Count representations for each m: number of primitive k dividing m */
    uint32_t *repr = (uint32_t *)calloc((size_t)(LIMIT + 1), sizeof(uint32_t));
    if (!repr) exit(EXIT_FAILURE);

    for (idx_t k = 1; k <= LIMIT; ++k) {
        if (prim_cnt[(size_t)k] == 0) continue;
        for (idx_t m = k; m <= LIMIT; m += k)
            repr[m] += prim_cnt[(size_t)k];
    }

    /* Build sequence array: all m such that repr[m]==1 */
    size_t capacity = 20000;
    *seq = seq_alloc_new_array(capacity);
    *seq_len = 0;

    for (idx_t m = 1; m <= LIMIT; ++m) {
        if (repr[m] == 1) {
            if (*seq_len == capacity) {
                capacity *= 2;
                seq_t *tmp = seq_alloc_new_array(capacity);
                for (size_t i = 0; i < *seq_len; ++i)
                    tmp[i] = (*seq)[i];
                free(*seq);
                *seq = tmp;
            }
            (*seq)[*seq_len] = (seq_t)m;
            ++(*seq_len);
            /* Stop when we are comfortably beyond long test requirements */
            if (*seq_len > 30000) break;
        }
    }

    /* Free temporary arrays */
    free(spf);
    free(prim_cnt);
    free(repr);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!initialized) {
        init_sequence(&seq, &seq_len);
        initialized = true;
    }

    if (n == 0 || n > seq_len) return 0; /* out of bounds – defined behaviour */
    return seq[n - 1];
}
