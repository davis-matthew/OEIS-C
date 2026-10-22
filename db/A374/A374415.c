/*
 * A374415  a(n) is the number of ways n can be written as a sum of a
 *          practical number and two octagonal numbers.
 * https://oeis.org/A374415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as the sum of a practical
 *            number and two octagonal numbers
 * Range:     u8 n <= 6740, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Integer power for idx_t (used only for small exponents) */
static idx_t pow_uint(idx_t base, int exp) {
    idx_t r = 1;
    while (exp-- > 0) r *= base;
    return r;
}

/* Test whether n is a practical number.
   Practical numbers: 1 and every even number whose prime factors satisfy the
   condition that each prime p_i (i >= 2) is at most 1 plus the sum of divisors
   of the product of the previous prime powers. */
static bool is_practical(idx_t n) {
    if (n == 1) return true;
    if ((n & 1ULL) == 1) return false;                /* must be even */

    idx_t rem = n;
    idx_t sigma = 1;                                  /* product of divisor sums so far */

    /* Handle factor 2 */
    int e = 0;
    while ((rem & 1ULL) == 0) { rem >>= 1; ++e; }
    idx_t divsum = (pow_uint(2ULL, e + 1) - 1);        /* 2^(e+1)-1 */
    sigma *= divsum;

    /* Handle odd prime factors */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p != 0) continue;
        if (p > sigma + 1) return false;              /* practical condition failed */
        e = 0;
        while (rem % p == 0) { rem /= p; ++e; }
        divsum = (pow_uint(p, e + 1) - 1) / (p - 1);
        sigma *= divsum;
    }

    /* Remaining prime > sqrt(n) */
    if (rem > 1) {
        if (rem > sigma + 1) return false;
        divsum = rem + 1;                             /*  (rem^2-1)/(rem-1) = rem+1 */
        sigma *= divsum;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Caches for practical numbers and octagonal numbers up to the largest
   value requested.  They grow as needed but stay valid for all subsequent
   calls. */
static idx_t *practical_nums = NULL;
static size_t practical_cnt = 0, practical_cap = 0;
static idx_t practical_last_cache = 0;

static idx_t *octagonal_nums = NULL;
static size_t oct_cnt = 0, oct_cap = 0;
static idx_t oct_last_k = 0;

/* Ensure practical numbers are generated up to N. */
static void ensure_practicals_upto(idx_t N) {
    if (N <= practical_last_cache) return;

    if (practical_cap == 0) {
        practical_cap = 64;
        practical_nums = (idx_t *)malloc(practical_cap * sizeof(idx_t));
    }

    for (idx_t x = practical_last_cache + 1; x <= N; ++x) {
        if (is_practical(x)) {
            if (practical_cnt == practical_cap) {
                practical_cap <<= 1;
                practical_nums = (idx_t *)realloc(practical_nums,
                                                  practical_cap * sizeof(idx_t));
            }
            practical_nums[practical_cnt++] = x;
        }
    }
    practical_last_cache = N;
}

/* Ensure octagonal numbers (including 0) are generated up to N. */
static void ensure_octagonal_upto(idx_t N) {
    if (oct_cap == 0) {
        oct_cap = 64;
        octagonal_nums = (idx_t *)malloc(oct_cap * sizeof(idx_t));
    }

    /* Ensure 0 is present */
    if (oct_cnt == 0) {
        if (oct_cnt == oct_cap) {
            oct_cap <<= 1;
            octagonal_nums = (idx_t *)realloc(octagonal_nums,
                                              oct_cap * sizeof(idx_t));
        }
        octagonal_nums[oct_cnt++] = 0;
        oct_last_k = 0;
    }

    while (1) {
        idx_t k = oct_last_k + 1;
        idx_t val = k * (3 * k - 2);          /* k(3k-2) */
        if (val > N) break;
        if (oct_cnt == oct_cap) {
            oct_cap <<= 1;
            octagonal_nums = (idx_t *)realloc(octagonal_nums,
                                              oct_cap * sizeof(idx_t));
        }
        octagonal_nums[oct_cnt++] = val;
        oct_last_k = k;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  It is safe to call in any order.           */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1 */

    ensure_practicals_upto(n);
    ensure_octagonal_upto(n);

    seq_t ways = 0;

    for (size_t i = 0; i < practical_cnt; ++i) {
        idx_t p = practical_nums[i];
        if (p > n) break;
        idx_t rem = n - p;

        size_t lo = 0;
        size_t hi = oct_cnt - 1;
        while (lo <= hi) {
            idx_t sum = octagonal_nums[lo] + octagonal_nums[hi];
            if (sum == rem) {
                ++ways;                 /* unordered pair found */
                ++lo;
                if (hi == 0) break;
                --hi;
            } else if (sum < rem) {
                ++lo;
            } else {                    /* sum > rem */
                if (hi == 0) break;
                --hi;
            }
        }
    }
    return ways;
}
