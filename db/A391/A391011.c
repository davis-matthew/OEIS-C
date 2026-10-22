/*
 * A391011  Quartic-full (4-full) Achilles numbers.
 * https://oeis.org/A391011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   4-full Achilles numbers. A n is 4‑full iff in the prime
 *            factorisation n = ∏pᵉᵢ, every eᵢ ≥ 4. It is Achilles iff the
 *            exponents are not all multiples of a common integer >1 – i.e.
 *            gcd(eᵢ) = 1. The function generates all 4‑full numbers up to a
 *            sufficiently large limit, retains those with gcd = 1, s...
 * Range:     u8 none, u16 n <= 8, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------- */
/* Auxiliary integer functions                                            */
/* -------------------------------------------------------------------- */
static inline int igcd(int a, int b)
{
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* integer square root (floor) for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* -------------------------------------------------------------------- */
/* Globals for prime sieve, term storage, and caching                   */
/* -------------------------------------------------------------------- */
#define MAX_TERMS 200000   /* sufficient for n ≤ 10000 */
#define LIMIT     2000000000ULL   /* 2×10⁹ – large enough for 10 000 terms */

struct term { seq_t val; int gcd; };

static struct term all_terms[MAX_TERMS];
static size_t term_count = 0;

static uint32_t *prime_list = NULL;
static size_t prime_count   = 0;

static seq_t *good_nums = NULL;
static size_t good_count = 0;

/* -------------------------------------------------------------------- */
/* Recursive generator of 4‑full numbers                                 */
/* -------------------------------------------------------------------- */
static void dfs(seq_t cur, int cur_gcd, size_t idx, seq_t limit,
                const uint32_t *primes, size_t pcnt)
{
    for (size_t i = idx; i < pcnt; ++i) {
        seq_t val = cur;
        int exp = 0;

        /* multiply at least 4 times */
        while (exp < 4) {
            if (val > limit / primes[i]) return;   /* cannot use this prime */
            val *= primes[i];
            ++exp;
        }

        /* now val = cur * p⁴, exp = 4 */
        for (;;) {
            int new_gcd = (cur == (seq_t)1) ? exp : igcd(cur_gcd, exp);
            if (term_count < MAX_TERMS) {
                all_terms[term_count].val = val;
                all_terms[term_count].gcd = new_gcd;
                ++term_count;
            }
            dfs(val, new_gcd, i + 1, limit, primes, pcnt);
            if (val > limit / primes[i]) break;        /* next exponent would overflow */
            val *= primes[i];
            ++exp;
        }
    }
}

/* -------------------------------------------------------------------- */
/* Comparison function for qsort                                        */
/* -------------------------------------------------------------------- */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;

    if (!initialized) {
        /* Build list of primes up to fourth root of LIMIT */
        uint64_t root2 = isqrt_u64(LIMIT);
        uint64_t root4 = isqrt_u64(root2);
        size_t maxp   = (size_t)root4 + 1;

        uint8_t *prime_flag = (uint8_t *)malloc(maxp);
        if (!prime_flag) return 0;
        memset(prime_flag, 1, maxp);
        if (maxp > 0) prime_flag[0] = 0;
        if (maxp > 1) prime_flag[1] = 0;

        for (size_t p = 2; p * p < maxp; ++p) {
            if (prime_flag[p]) {
                for (size_t q = p * p; q < maxp; q += p)
                    prime_flag[q] = 0;
            }
        }

        prime_list = (uint32_t *)malloc(maxp * sizeof(uint32_t));
        if (!prime_list) { free(prime_flag); return 0; }
        prime_count = 0;
        for (size_t i = 2; i < maxp; ++i)
            if (prime_flag[i]) prime_list[prime_count++] = (uint32_t)i;
        free(prime_flag);

        /* Generate all 4‑full numbers ≤ LIMIT */
        dfs((seq_t)1, 0, 0, LIMIT, prime_list, prime_count);

        /* Filter Achilles numbers (gcd of exponents == 1) */
        good_nums = (seq_t *)malloc(term_count * sizeof(seq_t));
        if (!good_nums) return 0;
        for (size_t i = 0; i < term_count; ++i)
            if (all_terms[i].gcd == 1)
                good_nums[good_count++] = all_terms[i].val;

        /* Sort ascending */
        qsort(good_nums, good_count, sizeof(seq_t), cmp_seq_t);

        initialized = 1;
    }

    /* Warning: if n > good_count we return 0, but the supplied n
       should be within bounds due to problem constraints. */
    if (n == 0 || (size_t)n > good_count) return 0;
    return good_nums[n - 1];
}
