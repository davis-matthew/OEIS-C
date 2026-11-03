/*
 * A295798  a(n) is the number of divisors d of prime(n)^2 - 1 such that
 *          prime(n) + d is prime.
 * https://oeis.org/A295798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of prime(n)^2–1 such that prime(n)+d
 *            is prime. (OEIS A295798)
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 7933, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9695 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all divisors of (prime(n)^2−1) by
   first factoring (prime(n)−1) and (prime(n)+1) separately, merging the
   factorizations, then checking each divisor.  All helper functions
   are provided in the universal <../utils/...> headers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static void dfs_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, seq_t cur, seq_t p, seq_t *res)
{
    if (idx == cnt) {
        if (is_prime_big(p + cur))
            (*res)++;
        return;
    }
    seq_t mult = cur;
    for (size_t k = 0; k <= ex[idx]; ++k) {
        dfs_divisors(pr, ex, cnt, idx + 1, mult, p, res);
        mult *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* 1‑based index, retrieve the n‑th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* factor (p−1) and (p+1) – they are close to each other and easy to factor */
    uint64_t pr1[64], ex1[64], pr2[64], ex2[64];
    size_t cnt1 = 0, cnt2 = 0;
    factor_u64((uint64_t)p32 - 1ULL, pr1, ex1, &cnt1);
    factor_u64((uint64_t)p32 + 1ULL, pr2, ex2, &cnt2);

    /* merge factorizations into a single list for p^2−1 */
    uint64_t pr_comb[128], ex_comb[128];
    size_t tot = 0;

    /* copy first factorization */
    for (size_t i = 0; i < cnt1; ++i) {
        pr_comb[tot] = pr1[i];
        ex_comb[tot] = ex1[i];
        ++tot;
    }

    /* merge second factorization */
    for (size_t j = 0; j < cnt2; ++j) {
        bool found = false;
        for (size_t i = 0; i < cnt1; ++i) {
            if (pr2[j] == pr_comb[i]) {
                ex_comb[i] += ex2[j];
                found = true;
                break;
            }
        }
        if (!found) {
            pr_comb[tot] = pr2[j];
            ex_comb[tot] = ex2[j];
            ++tot;
        }
    }

    /* enumerate divisors and count those that satisfy the primality test */
    seq_t result = 0;
    dfs_divisors(pr_comb, ex_comb, tot, 0, (seq_t)1, p, &result);

    return result;
}
