/*
 * A326112  Numbers requiring 3 central factorizations to reach prime
 *          factorization.
 * https://oeis.org/A326112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m (m ≥ 2) for which the number of
 *            successive central factorizations needed to reach the prime
 *            factorization of m equals 3
 * Range:     u8 n <= 4, u16 n <= 3563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9706 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326112: Numbers requiring 3 central factorizations to reach prime factorization. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static int depth_seq(seq_t n);

static int depth_seq(seq_t n)
{
    /* depth(n) = 0 if n is prime.
       Otherwise depth(n) = 1 + max( depth(a), depth(b) )
       where a*b is the central factorization of n. */
    if (is_prime_big(n))
        return 0;

    seq_t s = floor_sqrt(n);                /* floor(sqrt(n)) */
    seq_t d = s;

    /* Find largest divisor d <= sqrt(n) with d > 1 and n % d == 0 */
    while (d >= (seq_t)2) {
        if ((n % d) == (seq_t)0)
            break;
        d = d - (seq_t)1;
    }
    seq_t a = d;
    seq_t b = n / d;

    int da = depth_seq(a);
    int db = depth_seq(b);
    return 1 + (da > db ? da : db);
}

static seq_t *terms = NULL;           /* array of computed a(n) values (1‑based) */
static size_t terms_len = 0;          /* number of elements stored */
static size_t terms_cap = 0;          /* allocated capacity of the array */
static idx_t next_check = 2;          /* next integer to test for depth = 3 */

seq_t a(idx_t n)
{
    /* Ensure the array is large enough to store at least n terms. */
    if (terms_cap == 0) {
        terms_cap = 128;
        terms = mem_new(seq_t, terms_cap);
        terms_len = 0;
        next_check = 2;
    }

    while (terms_len < (size_t)n) {
        seq_t m = (seq_t)next_check;
        if (depth_seq(m) == 3) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;                 /* grow capacity */
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = m;
        }
        next_check = (idx_t)((seq_t)next_check + (seq_t)1);
    }

    return terms[n - 1];
}
