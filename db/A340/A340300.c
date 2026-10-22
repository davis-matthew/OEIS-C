/*
 * A340300  a(n) is the number of iterations for n to reach 1 under the
 *          following scheme. If k == 0 (mod 3), then k -> k/3, if k == 1 (mod
 *          3) k -> 2k, and if k == 2 (mod 3) add the middle two divisors of k
 *          and divide the result by 3.
 * https://oeis.org/A340300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations for n to reach 1 under the scheme:
 *            if k % 3 == 0 -> k = k / 3 if k % 3 == 1 -> k = 2 * k if k % 3
 *            == 2 -> k = (d_m/2 + d_m/2+1) / 3 where d_1 < d_2 < ... < d_m
 *            are all divisors of k (m even)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compare function for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Generate all divisors of a 64‑bit integer n and return the sum
   of the two middle divisors. */
static seq_t middle_divisor_sum(uint64_t n) {
    /* Factorisation by trial division */
    uint64_t tmp = n;
    uint64_t primes[32];
    int exp[32];
    int pf = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pf] = p;
            exp[pf] = e;
            ++pf;
        }
    }
    if (tmp > 1) {
        primes[pf] = tmp;
        exp[pf] = 1;
        ++pf;
    }

    /* Generate divisors */
    size_t cap = 64;
    size_t len = 1;
    seq_t *divs = (seq_t *)malloc(cap * sizeof(seq_t));
    divs[0] = 1;

    for (int i = 0; i < pf; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t old_len = len;
        seq_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= (seq_t)p;
            for (size_t j = 0; j < old_len; ++j) {
                if (len >= cap) {
                    cap *= 2;
                    divs = (seq_t *)realloc(divs, cap * sizeof(seq_t));
                }
                divs[len++] = divs[j] * p_pow;
            }
        }
    }

    /* Sort divisors */
    qsort(divs, len, sizeof(seq_t), cmp_seq);

    /* m is even by definition for k % 3 == 2 */
    size_t mid1 = len / 2 - 1;
    size_t mid2 = len / 2;
    seq_t sum = divs[mid1] + divs[mid2];
    free(divs);
    return sum / 3u;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t k = (seq_t)n;
    seq_t cnt = 0;
    while (k != 1u) {
        seq_t mod = k % 3u;
        if (mod == 0u) {
            k = k / 3u;
        } else if (mod == 1u) {
            k = k * 2u;
        } else { /* mod == 2 */
            uint64_t nk = (uint64_t)k;          /* assume fits in 64‑bit */
            seq_t sum = middle_divisor_sum(nk);
            k = sum;
        }
        ++cnt;
    }
    return cnt;
}
