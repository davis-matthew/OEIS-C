/*
 * A344589  Number of divisors d of n for which A011772(d) < A011772(n), where
 *          A011772(n) is the smallest number m such that m(m+1)/2 is
 *          divisible by n.
 * https://oeis.org/A344589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} [A011772(d) < A011772(n)]
 * Range:     u8 n <= 1471, u16 n <= 9932, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A344589: number of divisors d of n
   for which the smallest m such that m(m+1)/2 is divisible by d
   is less than that for n.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Local cache for A011772 values. */
static seq_t *tri_cache = NULL;    /* tri_cache[i] = A011772(i), 0=unknown */
static idx_t  tri_cache_size = 0;  /* next index to be initialized (size */

/* Small prime table for factoring up to sqrt(10000) (~100). */
static uint64_t prime_tbl[200];
static int prime_cnt = 0;

/* Initialise the prime table once. */
static void init_primes(void) {
    if (prime_cnt) return;
    const int LIM = 200;
    bool is_prime[LIM + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIM; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= LIM; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= LIM; ++i)
        if (is_prime[i])
            prime_tbl[prime_cnt++] = (uint64_t)i;
}

/* Compute A011772(n) (smallest m with m(m+1)/2 ≡ 0 (mod n)). */
static seq_t get_tri(idx_t n) {
    /* Ensure cache large enough. */
    if (tri_cache == NULL) {
        /* Compute up to n. */
        idx_t new_size = n + 1;
        tri_cache = (seq_t*)malloc(sizeof(seq_t) * new_size);
        if (!tri_cache) return 0;            /* allocation failed */
        for (idx_t i = 0; i < new_size; ++i)
            tri_cache[i] = 0;
        for (idx_t i = 1; i < new_size; ++i) {
            uint64_t k = 1;
            while (1) {
                __int128 prod = (__int128)k * (k + 1);
                uint64_t T = (uint64_t)(prod / 2);
                if (T % i == 0) {
                    tri_cache[i] = (seq_t)k;
                    break;
                }
                ++k;
            }
        }
        tri_cache_size = new_size;
    } else if (n >= tri_cache_size) {
        idx_t old_size = tri_cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(tri_cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;                     /* allocation failed */
        tri_cache = tmp;
        for (idx_t i = old_size; i < new_size; ++i)
            tri_cache[i] = 0;
        for (idx_t i = old_size; i < new_size; ++i) {
            uint64_t k = 1;
            while (1) {
                __int128 prod = (__int128)k * (k + 1);
                uint64_t T = (uint64_t)(prod / 2);
                if (T % i == 0) {
                    tri_cache[i] = (seq_t)k;
                    break;
                }
                ++k;
            }
        }
        tri_cache_size = new_size;
    }
    return tri_cache[n];
}

/* Factor n into primes and exponents (simple trial division). */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, int *cnt) {
    int c = 0;
    uint64_t tmp = n;
    for (int i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        if (tmp % prime_tbl[i] == 0) {
            int e = 0;
            while (tmp % prime_tbl[i] == 0) {
                tmp /= prime_tbl[i];
                ++e;
            }
            pr[c] = prime_tbl[i];
            exp[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {
        pr[c] = tmp;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of all divisors of a number. */
static void rec_divs(int idx, uint64_t cur, uint64_t *pr, int *exp,
                     int total, seq_t tn, uint64_t *cnt_ans) {
    if (idx == total) {
        seq_t td = get_tri(cur);
        if (td < tn)
            ++(*cnt_ans);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t mul = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        rec_divs(idx + 1, cur * mul, pr, exp, total, tn, cnt_ans);
        mul *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not defined, but harmless */
    init_primes();                  /* ensure prime table ready */

    seq_t tn = get_tri(n);          /* precompute A011772(n) */

    uint64_t primes_arr[32];
    int exp_arr[32];
    int cnt = 0;
    factor_uint64(n, primes_arr, exp_arr, &cnt);

    uint64_t answer = 0;
    rec_divs(0, 1, primes_arr, exp_arr, cnt, tn, &answer);

    return (seq_t)answer;
}
