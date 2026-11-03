/*
 * A381855  Starting from prime(n), a(n) is the minimum number > 1 of
 *          consecutive primes whose sum is the lesser of a twin prime pair.
 * https://oeis.org/A381855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9874 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

/* The function finds, for index n ≥ 1, the smallest k > 1 such that
 * the sum of k consecutive primes starting with the n‑th prime equals
 * the smaller member of a twin‑prime pair; it returns that k. */

/* Prime storage -------------------------------------------------------- */
static uint64_t       *prime_arr = NULL;
static size_t          prime_cnt = 0;
static size_t          prime_cap = 0;
static uint64_t        next_candidate = 2;

/* Ensure the array can hold at least 'cap' primes. */
static void ensure_prime_cap(size_t cap)
{
    if (prime_cap < cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        if (new_cap < cap) new_cap = cap;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = new_cap;
    }
}

/* Append a new prime to the array. */
static void append_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_arr[prime_cnt++] = p;
}

/* Primality test for 64‑bit values. */
static bool is_64bit_prime(uint64_t x)
{
    return is_prime((idx_t)x);
}

/* Generate the next prime and store it. */
static void generate_next_prime(void)
{
    uint64_t p = next_candidate;
    while (1) {
        if (p < 2) p = 2;
        if (p == 2) {
            append_prime(p);
            next_candidate = 3;
            return;
        }
        if (is_64bit_prime(p)) {
            append_prime(p);
            next_candidate = p + 2;      /* skip evens */
            return;
        }
        p += 2;
    }
}

/* Ensure that at least 'need' primes (prime(1)…prime(need)) are available. */
static void ensure_primes_up_to(size_t need)
{
    while (prime_cnt < need) generate_next_prime();
}

/* Memoisation storage ----------------------------------------------- */
static seq_t *memo = NULL;
static idx_t  memo_cap = 0;

/* Resize memo array to hold at least 'cap' entries. */
static void ensure_memo_cap(idx_t cap)
{
    if (cap > memo_cap) {
        seq_t *tmp = mem_resize(seq_t, memo, (size_t)cap);
        memo = tmp;
        mem_set(memo + (size_t)memo_cap, 0, (size_t)(cap - memo_cap) * sizeof(seq_t));
        memo_cap = cap;
    }
}

/* Sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* invalid index */

    /* make memo big enough */
    ensure_memo_cap(n);

    /* cached value? */
    if (memo[n - 1] != 0) return memo[n - 1];

    /* obtain at least the n‑th prime */
    ensure_primes_up_to((size_t)n);

    const size_t start = (size_t)(n - 1);   /* zero‑based index of prime(n) */
    seq_t sum = 0;
    size_t k   = 0;

    while (1) {
        /* guarantee prime list covers required index */
        if (start + k >= prime_cnt) generate_next_prime();

        sum += (seq_t)prime_arr[start + k];
        ++k;

        if (k > 1) {
            bool prime_sum, prime_sum2;
            seq_t sum2 = sum + (seq_t)2;

            if (sum <= (seq_t)UINT64_MAX) {
                prime_sum  = is_64bit_prime((uint64_t)sum);
                prime_sum2 = is_64bit_prime((uint64_t)sum2);
            } else {
                prime_sum  = is_prime_big(sum);
                prime_sum2 = is_prime_big(sum2);
            }

            if (prime_sum && prime_sum2) {
                memo[n - 1] = (seq_t)k;
                return memo[n - 1];
            }
        }
    }
}
