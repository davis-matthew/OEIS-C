/*
 * A261581  Primes such that z(p) is not divisible by 4 where z(n) is
 *          A214028(n), the smallest k such that n divides A000129(k), the
 *          k-th Pell number.
 * https://oeis.org/A261581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime such that z(p) is not divisible by 4, where
 *            z(p) is the smallest k with p | Pell(k)
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Multiply two 2x2 matrices (a * b) modulo mod, result in r */
static inline void mul_matrix(seq_t r[4], const seq_t a[4], const seq_t b[4], seq_t mod) {
    seq_t a0=a[0], a1=a[1], a2=a[2], a3=a[3];
    seq_t b0=b[0], b1=b[1], b2=b[2], b3=b[3];
    r[0] = (a0*b0 + a1*b2) % mod;
    r[1] = (a0*b1 + a1*b3) % mod;
    r[2] = (a2*b0 + a3*b2) % mod;
    r[3] = (a2*b1 + a3*b3) % mod;
}

/* Return true if matrix M^exp (mod p) has entry [0][1] == 0.
   M is fixed [[2,1],[1,0]].
*/
static inline bool zero_at_index(uint64_t p, idx_t exp) {
    seq_t res[4]  = {1,0,0,1};
    seq_t base[4] = {(seq_t)(2 % p), (seq_t)(1 % p), (seq_t)(1 % p), (seq_t)(0)};
    seq_t tmp[4];
    idx_t e = exp;
    while (e) {
        if (e & 1) {
            mul_matrix(tmp, res, base, (seq_t)p);
            for (int i=0;i<4;i++) res[i]=tmp[i];
        }
        mul_matrix(tmp, base, base, (seq_t)p);
        for (int i=0;i<4;i++) base[i]=tmp[i];
        e >>= 1;
    }
    return res[1] == 0;
}

/* Compute the rank of apparition z(p) for a prime p (p != 2). */
static inline idx_t rank_of_apparition(uint64_t p) {
    if (p == 2) return 2;                     // special case
    int leg;                                 // Legendre symbol (2|p)
    int mod8 = p & 7;
    leg = (mod8 == 1 || mod8 == 7) ? 1 : -1;
    idx_t N = (idx_t)(p - leg);              // p - (2|p)

    /* Factor N */
    idx_t tmp = N;
    idx_t factors[20];
    int fcnt = 0;
    if (tmp % 2 == 0) {
        factors[fcnt++] = 2;
        while (tmp % 2 == 0) tmp /= 2;
    }
    for (idx_t i = 3; i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            factors[fcnt++] = i;
            while (tmp % i == 0) tmp /= i;
        }
    }
    if (tmp > 1) factors[fcnt++] = tmp;

    idx_t d = N;
    for (int idx = 0; idx < fcnt; ++idx) {
        idx_t f = factors[idx];
        while (d % f == 0) {
            idx_t cand = d / f;
            if (zero_at_index(p, cand))
                d = cand;
            else
                break;
        }
    }
    return d;
}

static seq_t *good = NULL;
static size_t good_cap = 0;
static size_t good_cnt = 0;
static uint64_t last_checked = 1;  // last prime considered

/* Ensure the array 'good' has enough capacity. */
static void grow_good(void) {
    if (good_cap == 0) {
        good_cap = 16;
        good = mem_new(seq_t, good_cap);
    } else if (good_cnt >= good_cap) {
        good_cap <<= 1;
        good = mem_resize(seq_t, good, good_cap);
    }
}

/* Build the list of good primes until it contains at least 'need' entries. */
static void build_good_until(size_t need) {
    if (good_cnt >= need) return;
    while (good_cnt < need) {
        last_checked = next_prime(last_checked);
        idx_t z = rank_of_apparition(last_checked);
        if (z % 4 != 0) {
            grow_good();
            good[good_cnt++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                // undefined for offset 1
    build_good_until((size_t)n);
    return good[n - 1];
}
