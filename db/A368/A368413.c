/*
 * A368413  Number of factorizations of n into positive integers > 1 such that
 *          it is not possible to choose a different prime factor of each
 *          factor.
 * https://oeis.org/A368413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into integers >1
 *            where no distinct prime can be selected from each factor
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>   /* for sqrt, but used only in compile-time cast */

/* max n that will ever be queried */
#define MAX_QUERY 10000

/* maximum number of distinct primes up to MAX_QUERY */
#define PRIME_CAP 1225
#define DIV_CAP 128

static uint32_t primes[PRIME_CAP];
static size_t prime_cnt = 0;
static int prime_index[MAX_QUERY + 1];

static uint16_t div_cnt[MAX_QUERY + 1];
static uint16_t divs[MAX_QUERY + 1][DIV_CAP];

static uint16_t cur_factors[64];
static uint16_t factor_prime_sets[64][16];
static uint8_t factor_prime_cnt[64];
static size_t cur_k = 0;
static bool used_prime[2000];
static seq_t a_val;

/* ---------- Prime utilities ---------- */
static void build_primes(uint32_t limit)
{
    bool seen[MAX_QUERY + 1];
    memset(seen, true, sizeof(seen));
    seen[0] = seen[1] = false;
    prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (seen[i]) {
            if (prime_cnt >= PRIME_CAP) break;
            primes[prime_cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                seen[j] = false;
        }
    }
    for (int i = 0; i <= limit; ++i) prime_index[i] = -1;
    for (size_t i = 0; i < prime_cnt; ++i) prime_index[primes[i]] = (int)i;
}

/* ---------- Divisor precomputation ---------- */
static void build_divisors(uint32_t limit)
{
    memset(div_cnt, 0, sizeof(div_cnt));
    for (uint32_t i = 2; i <= limit; ++i)
        for (uint32_t j = i; j <= limit; j += i)
            divs[j][div_cnt[j]++] = (uint16_t)i;
}

/* ---------- Distinct prime factor extraction ---------- */
static int distinct_prime_factors(uint32_t val, uint32_t out[16])
{
    int cnt = 0;
    uint32_t temp = val;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            out[cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && cnt < 16) out[cnt++] = temp;
    return cnt;
}

/* ---------- Matching for distinct prime assignment ---------- */
static bool backtrack_match(int idx)
{
    if ((size_t)idx == cur_k) return true;
    for (uint8_t j = 0; j < factor_prime_cnt[idx]; ++j) {
        uint16_t p = factor_prime_sets[idx][j];
        if (!used_prime[p]) {
            used_prime[p] = true;
            if (backtrack_match(idx + 1)) {
                used_prime[p] = false;
                return true;
            }
            used_prime[p] = false;
        }
    }
    return false;
}

/* ---------- Recursion over factorizations ---------- */
static void recurse(uint32_t rem, uint32_t start, size_t depth)
{
    if (rem == 1) {
        cur_k = depth;
        /* Factor each current factor and fill prime sets */
        for (size_t i = 0; i < cur_k; ++i) {
            uint32_t flist[16];
            factor_prime_cnt[i] = distinct_prime_factors(cur_factors[i], flist);
            for (uint8_t j = 0; j < factor_prime_cnt[i]; ++j) {
                /* Map prime to its global index for used[] array */
                uint32_t p = flist[j];
                factor_prime_sets[i][j] = (uint16_t)prime_index[p];
            }
        }
        /* Try to find a distinct prime assignment */
        memset(used_prime, 0, sizeof(used_prime));
        if (!backtrack_match(0)) a_val++;
        return;
    }
    for (size_t i = 0; i < div_cnt[rem]; ++i) {
        uint16_t f = divs[rem][i];
        if (f < start) continue;
        cur_factors[depth] = f;
        recurse(rem / f, f, depth + 1);
    }
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint32_t N = (uint32_t)n;
    if (N > MAX_QUERY) {
        /* In case larger queries are requested, but constraints guarantee <=10000 */
        build_primes(N);
        build_divisors(N);
    } else {
        /* Build only once for the maximum needed */
        static bool initialized = false;
        if (!initialized) {
            build_primes(MAX_QUERY);
            build_divisors(MAX_QUERY);
            initialized = true;
        }
        if (N > MAX_QUERY) {
            /* Build extended tables for the larger n */
            build_primes(N);
            build_divisors(N);
        }
    }

    a_val = 0;
    recurse(N, 2, 0);
    return a_val;
}
