/*
 * A285510  Numbers k such that the average of the squarefree divisors of k is
 *          an integer.
 * https://oeis.org/A285510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that * the average of the
 *            squarefree divisors of k is an integer. * Equivalently, (∏_(p |
 *            k) (1+p)) is divisible by 2^ω(k), * where ω(k) is the number of
 *            distinct prime divisors of k
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SPF_MAX 200000     /* upper bound large enough for n <= 10000 */

/* smallest prime factor array, 0 means prime */
static uint32_t spf[SPF_MAX + 1] = {0};
static bool spf_built = false;

/* Build sieve for smallest prime factors up to SPF_MAX */
static void build_spf(void) {
    for (uint32_t i = 2; i <= SPF_MAX; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= SPF_MAX) {
                for (uint64_t j = (uint64_t)i * i; j <= SPF_MAX; j += i) {
                    if (spf[(uint32_t)j] == 0)
                        spf[(uint32_t)j] = i;
                }
            }
        }
    }
    spf_built = true;
}

/* Return true if the average of the squarefree divisors of k is integer */
static bool good_k(uint64_t k) {
    if (k == 0) return false;
    int omega = 0;
    unsigned __int128 prod = 1;
    uint64_t tmp = k;
    while (tmp > 1) {
        uint32_t p = spf[(uint32_t)tmp];
        omega++;
        prod *= (unsigned __int128)(p + 1);
        while (tmp % p == 0) tmp /= p;
    }
    unsigned __int128 pow2 = (unsigned __int128)1 << omega;
    return prod % pow2 == 0;
}

seq_t a(idx_t n) {
    if (!spf_built) build_spf();

    seq_t result = 0;
    if (n == 0) return result;   /* out of OEIS offset, not used */

    uint64_t k = 1;
    uint64_t cnt = 0;
    while (cnt < n) {
        if (good_k(k)) {
            ++cnt;
            if (cnt == n) {
                result = (seq_t)k;
                break;
            }
        }
        ++k;
        /* safety: if k exceeds our sieve, enlarge sieve */
        if (k > SPF_MAX) {
            /* unlikely for n <= 10000, but handle */
            /* In practice, this path is not needed */
            /* Here we simply use trial division */
            uint64_t t = k;
            int omega = 0;
            unsigned __int128 prod = 1;
            while (t > 1) {
                uint64_t p = 2;
                while (p * p <= t && t % p != 0) ++p;
                if (p * p > t) p = t;
                if (p == t) p = t;
                omega++;
                prod *= (unsigned __int128)(p + 1);
                while (t % p == 0) t /= p;
            }
            unsigned __int128 pow2 = (unsigned __int128)1 << omega;
            if (prod % pow2 == 0) {
                ++cnt;
                if (cnt == n) {
                    result = (seq_t)k;
                    break;
                }
            }
        }
    }
    return result;
}
