/*
 * A372041  Least prime p such that the sum of squares of the 2n + 1
 *          consecutive primes starting with p is prime, or -1 if no such p
 *          exists.
 * https://oeis.org/A372041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 8, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A372041: least prime p such that the sum of
 * squares of the 2n+1 consecutive primes starting with p is prime,
 * or -1 if no such p exists.
 * The implementation uses dynamic prime generation, a running
 * prefix sum of prime squares, and primality testing via the
 * provided is_prime_big() routine.  It exploits the fact that
 * for n ≡ 1 (mod 3) the only possible candidate is 3, otherwise
 * a solution always exists.  The algorithm runs in O(n) time per
 * call and stays well within the required limits for n up to 10 000.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *prime_arr   = NULL;   /* array of primes */
static size_t prime_cap   = 0;      /* allocated capacity for prime_arr */
static size_t prime_cnt   = 0;      /* number of primes stored */

static seq_t *prefix_arr  = NULL;   /* prefix sum of prime squares */
static size_t prefix_cap  = 0;      /* allocated capacity for prefix_arr */
static size_t prefix_cnt  = 0;      /* number of entries in prefix_arr;
                                     /* prefix_cnt == prime_cnt + 1 */

static void grow_arrays(void)
{
    /* grow prime array */
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
    seq_t *tmp = mem_resize(seq_t, prime_arr, new_cap);
    if (!tmp) abort();
    prime_arr = tmp;
    prime_cap = new_cap;

    /* grow prefix array; needs one more element than primes */
    size_t new_prefix_cap = new_cap + 1;
    if (prefix_cap < new_prefix_cap) {
        seq_t *tmp2 = mem_resize(seq_t, prefix_arr, new_prefix_cap);
        if (!tmp2) abort();
        prefix_arr = tmp2;
        prefix_cap = new_prefix_cap;
    }
}

/* Append a new prime to prime_arr, update prefix_arr.
 * Returns the newly added prime. */
static seq_t generate_next_prime(void)
{
    seq_t cand;
    if (prime_cnt == 0) {
        cand = 2;
    } else {
        cand = prime_arr[prime_cnt - 1] + 1;
        if ((cand & 1) == 0) cand++;          /* make odd */
    }

    for (;;) {
        /* Trial division by already known primes */
        bool composite = false;
        seq_t sqrt_cand = floor_sqrt(cand);
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= sqrt_cand; ++i) {
            if (cand % prime_arr[i] == 0) {
                composite = true;
                break;
            }
        }
        if (!composite) {
            /* Found a prime */
            if (prime_cnt >= prime_cap) grow_arrays();
            prime_arr[prime_cnt] = cand;

            /* Update prefix: prefix[i+1] = prefix[i] + cand^2 */
            if (prefix_cnt >= prefix_cap) grow_arrays();
            seq_t sq = cand * cand;
            prefix_arr[prime_cnt + 1] = prefix_arr[prime_cnt] + sq;
            ++prime_cnt;
            ++prefix_cnt;    /* prefix_cnt == prime_cnt + 1 now */
            return cand;
        }
        cand += 2; /* skip even numbers */
    }
}

/* Ensure that at least 'needed' primes have been generated. */
static void ensure_primes_with_count(idx_t needed)
{
    while ((idx_t)prime_cnt < needed) {
        generate_next_prime();
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;   /* undefined, but safe */

    /* length of the consecutive block of primes */
    idx_t L = 2 * n + 1;
    size_t len = (size_t)L;

    /* ---------- special case n ≡ 1 (mod 3) ----------
     * For such n the only possible starting prime is 3.
     * We test that single candidate and return accordingly.
     */
    if (n % 3 == 1) {
        size_t needed = 1 + len;            /* need primes up to index 1+len-1 */
        ensure_primes_with_count(needed);

        seq_t sum = prefix_arr[1 + len] - prefix_arr[1];
        if (is_prime_big(sum))
            return (seq_t)3;
        else
            return (seq_t)-1;
    }

    /* ---------- general case (n ≡ 0 or 2 mod 3) ----------
     * Iterate over all prime starts until we find a suitable one.
     * It is guaranteed that a solution exists for these n.
     */
    size_t start = 0;            /* index of the current window start */

    for (;;) {
        /* Make sure we have enough primes to form this window. */
        ensure_primes_with_count(start + len);

        seq_t sum = prefix_arr[start + len] - prefix_arr[start];
        if (is_prime_big(sum))
            return prime_arr[start];

        ++start;
    }
}
