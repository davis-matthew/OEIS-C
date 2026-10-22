/*
 * A308644  Number of ways to write n as (3^a*5^b)^2 + c*(3c+1)/2 +
 *          d*(7d+1)/2, where a and b are nonnegative integers, and c and d
 *          are integers.
 * https://oeis.org/A308644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count of representations of n as (3^a * 5^b)^2 + c*(3c+1)/2 +
 *            d*(7d+1)/2 where a,b ≥ 0 and c,d ∈ ℤ
 * Range:     u8 n <= 13, u16 n <= 1318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for seq_t (returns floor(sqrt(x))) */
static inline seq_t isqrt_seq(seq_t x) {
    unsigned long long low = 0, high = (unsigned long long)x;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned long long prod = mid * mid;
        if (prod == x) return (seq_t)mid;
        if (prod < x) low = mid + 1;
        else high = mid - 1;
    }
    return (seq_t)high;
}

/* Test whether rem2 can be expressed as d*(7d+1)/2 for some integer d. */
static inline bool has_d(seq_t rem2) {
    seq_t D = 1 + 56 * rem2;
    seq_t sqrtD = isqrt_seq(D);
    if ((unsigned long long)sqrtD * sqrtD != D) return false;
    unsigned long long r = (unsigned long long)sqrtD;
    return r % 14ULL == 1ULL || r % 14ULL == 13ULL;
}

/* Count representations of rem as c*(3c+1)/2 + d*(7d+1)/2. */
static inline seq_t count_rep_rem(seq_t rem) {
    seq_t count = 0;
    /* find maximum k such that f(k) <= rem or f(-k) <= rem */
    int k = 0;
    while (1) {
        seq_t fpos = ((seq_t)k * (((seq_t)3 * k) + 1)) / 2;
        seq_t fneg = ((seq_t)k * (((seq_t)3 * k) - 1)) / 2;
        if (k > 0 && fpos > rem && fneg > rem) break;
        ++k;
    }
    int maxK = k; /* exclusive */
    for (int kk = 0; kk < maxK; ++kk) {
        /* c = kk (non‑negative) */
        seq_t fPos = ((seq_t)kk * (((seq_t)3 * kk) + 1)) / 2;
        if (rem >= fPos && has_d(rem - fPos)) ++count;
        /* c = -kk, except for kk = 0 to avoid double counting */
        if (kk > 0) {
            seq_t fNeg = ((seq_t)kk * (((seq_t)3 * kk) - 1)) / 2;
            if (rem >= fNeg && has_d(rem - fNeg)) ++count;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t n_val = (seq_t)n;
    seq_t sqrtN = isqrt_seq(n_val);          /* floor(sqrt(n)) */

    seq_t result = 0;

    seq_t pow3 = 1;
    while (pow3 <= sqrtN) {
        seq_t pow5 = 1;
        while (pow5 <= sqrtN / pow3) {
            seq_t p = pow3 * pow5;           /* p = 3^a * 5^b */
            seq_t t = p * p;                 /* t = p^2 */
            if (t <= n_val) {
                seq_t rem = n_val - t;
                result += count_rep_rem(rem);
            }
            if (pow5 > sqrtN / 5) break;
            pow5 *= 5;
        }
        if (pow3 > sqrtN / 3) break;
        pow3 *= 3;
    }

    return result;
}
