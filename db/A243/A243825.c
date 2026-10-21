/*
 * A243825  Numbers n such that every divisor greater than 1 contains the
 *          digit 0.
 * https://oeis.org/A243825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose every divisor >1 *
 *            contains the digit 0. Computed by incremental search
 * Range:     u8 n <= 4, u16 n <= 1709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve helpers --------------------------------- */

static uint32_t *primes = NULL;
static size_t   primes_cnt = 0, primes_cap = 0;
static uint32_t next_prime_candidate = 3;

/* integer square root of a 64‑bit number */
static inline uint32_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n) return (uint32_t)mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return (uint32_t)hi;
}

/* extend the prime table up to ‘limit’ */
static void ensure_prime_limit(uint32_t limit)
{
    if (primes_cnt == 0) {
        primes_cap = 16;
        primes     = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) exit(1);
        primes[0] = 2;
        primes_cnt = 1;
        next_prime_candidate = 3;
    }
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > next_prime_candidate) break;
            if (next_prime_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[primes_cnt++] = next_prime_candidate;
        }
        next_prime_candidate += 2;
    }
}

/* ---------- Utilities ------------------------------------------- */

/* true if ‘x’ contains a decimal digit 0 */
static inline bool has_zero_digit(seq_t x)
{
    while (x > 0) {
        if ((x % 10ULL) == 0) return true;
        x /= 10ULL;
    }
    return false;
}

/* prime factorisation */
static void factor(uint64_t n,
                   const uint32_t *primes_arr, size_t primes_arr_cnt,
                   uint32_t *fp, uint8_t *fe, size_t *fc)
{
    size_t cnt = 0;
    uint64_t m = n;
    uint32_t sqrt_m = isqrt_u64(m);
    for (size_t i = 0; i < primes_arr_cnt && primes_arr[i] <= sqrt_m; ++i) {
        uint32_t p = primes_arr[i];
        if (m % p == 0) {
            uint8_t e = 0;
            do { m /= p; ++e; } while (m % p == 0);
            fp[cnt] = p; fe[cnt++] = e;
            sqrt_m = isqrt_u64(m);
        }
    }
    if (m > 1) {
        fp[cnt] = (uint32_t)m;
        fe[cnt++] = 1;
    }
    *fc = cnt;
}

/* divisor enumeration */
static bool check_divisors_rec(const uint32_t *p, const uint8_t *e,
                               size_t pcnt, size_t idx, seq_t cur)
{
    if (idx == pcnt) {
        if (cur == 1) return true;          /* skip divisor 1 */
        return has_zero_digit(cur);
    }
    seq_t val = cur;
    for (uint8_t k = 0; k <= e[idx]; ++k) {
        if (!check_divisors_rec(p, e, pcnt, idx + 1, val)) return false;
        val *= p[idx];
    }
    return true;
}

/* test whether a number satisfies the property */
static bool is_valid_number(seq_t num)
{
    if (num <= 1) return false;
    if (!has_zero_digit(num)) return false;   /* number itself must contain 0 */

    uint64_t n = (uint64_t)num;
    uint32_t sqrt_n = isqrt_u64(n);
    ensure_prime_limit(sqrt_n);

    uint32_t fp[16];
    uint8_t  fe[16];
    size_t   fc;
    factor(n, primes, primes_cnt, fp, fe, &fc);

    /* prime factors must all contain 0 */
    for (size_t i = 0; i < fc; ++i) {
        if (!has_zero_digit(fp[i])) return false;
    }

    /* check all non‑trivial divisors */
    return check_divisors_rec(fp, fe, fc, 0, 1);
}

/* ---------- Public function ------------------------------------ */

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t  last_checked = 0;

    while (seq_len < n) {
        ++last_checked;
        if (is_valid_number((seq_t)last_checked)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 16;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(1);
            }
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
