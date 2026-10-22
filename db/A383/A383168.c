/*
 * A383168  Triangle T(n,k) read by rows: For closed chains of identical
 *          regular m-gons with connecting inner vertices lying n vertices
 *          apart, the n-th row lists the possible m in ascending order; n>=0,
 *          1<=k<=d(8+4n).
 * https://oeis.org/A383168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A383168.
   For n >= 1, a(n) is the flattened triangle T(n_row,k) defined by:
     T(r,k) = 4 + 2r + d_k
   where d_k is the k-th (ascending) divisor of 8 + 4r.
   The function walks rows until it reaches the requested term. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Smallest prime factor (trial division). */
static uint64_t smallest_prime_factor(uint64_t n) {
    if ((n & 1) == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return p;
    }
    return n;  /* n is prime */
}

/* Recursive generation of all divisors from prime factorization. */
static void gen_divs(int idx, uint64_t prod,
                     const uint64_t *primes, const uint8_t *exps,
                     int pf_cnt, uint64_t *divs, int *divcnt) {
    if (idx == pf_cnt) {
        divs[(*divcnt)++] = prod;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t cur = prod;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, cur, primes, exps, pf_cnt, divs, divcnt);
        cur *= p;
    }
}

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Main function required by the build. */
seq_t a(idx_t n) {
    idx_t target = n;              /* 1‑based index */
    idx_t row = 0;
    for (;;) {
        uint64_t m = 8ULL + 4ULL * row;          /* 8 + 4r */
        /* factorization to get divisor count */
        uint64_t tmp = m;
        uint64_t divcnt = 1;
        uint64_t primes[12];
        uint8_t exps[12];
        int pf_cnt = 0;
        while (tmp > 1) {
            uint64_t p = smallest_prime_factor(tmp);
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
            divcnt *= (uint64_t)(e + 1);
        }
        if (target <= divcnt) {          /* answer is in this row */
            /* regenerate primes and exps (repeat is fine here) */
            tmp = m;
            pf_cnt = 0;
            while (tmp > 1) {
                uint64_t p = smallest_prime_factor(tmp);
                uint8_t e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                primes[pf_cnt] = p;
                exps[pf_cnt] = e;
                ++pf_cnt;
            }
            /* generate all divisors */
            uint64_t divs[256];
            int dcnt = 0;
            gen_divs(0, 1ULL, primes, exps, pf_cnt, divs, &dcnt);
            /* sort divisors */
            qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);
            uint64_t kth_div = divs[target - 1];
            seq_t base = (seq_t)(4ULL + 2ULL * row);
            return base + (seq_t)kth_div;
        } else {
            target -= divcnt;   /* skip this row */
            ++row;
        }
    }
}
